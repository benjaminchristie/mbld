#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 160
#define HEIGHT 80
#define FOV 40.0
#define CUBES_PER_AXIS 3
#define CUBE_SIZE 0.3f
#define SPACING 0.02f
#define NUM_CUBES (CUBES_PER_AXIS * CUBES_PER_AXIS * CUBES_PER_AXIS)

typedef struct { float x, y, z; } Vec3;
typedef struct { Vec3 min, max; } Box;

const char* get_color(Vec3 normal) {
    if (normal.z > 0.9) return "\033[91m"; // Red (front)
    if (normal.z < -0.9) return "\033[38;5;208m"; // Orange (back)
    if (normal.x > 0.9) return "\033[92m"; // Green (right)
    if (normal.x < -0.9) return "\033[94m"; // Blue (left)
    if (normal.y > 0.9) return "\033[97m"; // White (top)
    if (normal.y < -0.9) return "\033[93m"; // Yellow (bottom)
    return "\033[0m";
}

Vec3 vec_add(Vec3 a, Vec3 b) { return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }
Vec3 vec_sub(Vec3 a, Vec3 b) { return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z}; }
Vec3 vec_scale(Vec3 a, float s) { return (Vec3){a.x * s, a.y * s, a.z * s}; }
float vec_dot(Vec3 a, Vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
Vec3 vec_norm(Vec3 v) {
    float len = sqrtf(vec_dot(v, v));
    return vec_scale(v, 1.0f / len);
}

int ray_box_intersect(Vec3 origin, Vec3 dir, Vec3 min, Vec3 max, float *t_min_out, Vec3 *hit_normal_out) {
    float tmin = -INFINITY, tmax = INFINITY;
    Vec3 normals[6] = {
        {-1,0,0}, {1,0,0}, {0,-1,0}, {0,1,0}, {0,0,-1}, {0,0,1}
    };
    Vec3 hit_normal = {0};

    float t1, t2;
    for (int axis = 0; axis < 3; axis++) {
        float o = (&origin.x)[axis];
        float d = (&dir.x)[axis];
        float min_val = (&min.x)[axis];
        float max_val = (&max.x)[axis];

        if (fabsf(d) < 1e-6) {
            if (o < min_val || o > max_val) return 0;
        } else {
            t1 = (min_val - o) / d;
            t2 = (max_val - o) / d;
            if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }
            if (t1 > tmin) {
                tmin = t1;
                hit_normal = d < 0 ? normals[axis*2+1] : normals[axis*2];
            }
            if (t2 < tmax) tmax = t2;
            if (tmin > tmax || tmax < 0) return 0;
        }
    }

    if (t_min_out) *t_min_out = tmin;
    if (hit_normal_out) *hit_normal_out = hit_normal;
    return 1;
}

void render_frame(Vec3 camera, Box *cubes, int num, Vec3 light_dir) {
    float aspect = (float)WIDTH / HEIGHT;
    float scale = tanf(FOV * 0.5 * M_PI / 180.0);

    for (int y = 0; y < HEIGHT; y += 2) {
        for (int x = 0; x < WIDTH; x++) {
            char top_char = ' ', bottom_char = ' ';
            const char *top_color = "\033[0m", *bottom_color = "\033[0m";

            for (int half = 0; half < 2; half++) {
                float py = (1 - 2 * (((y + half) + 0.5f) / HEIGHT)) * scale;
                float px = (2 * ((x + 0.5f) / WIDTH) - 1) * aspect * scale;
                Vec3 dir = vec_norm((Vec3){px, py, 1});
                float closest_t = INFINITY;
                Vec3 final_normal = {0};
                int hit = 0;

                for (int i = 0; i < num; i++) {
                    float t;
                    Vec3 normal;
                    if (ray_box_intersect(camera, dir, cubes[i].min, cubes[i].max, &t, &normal)) {
                        if (t < closest_t) {
                            closest_t = t;
                            final_normal = normal;
                            hit = 1;
                        }
                    }
                }

                if (hit) {
                    float brightness = vec_dot(final_normal, vec_scale(light_dir, -1));
                    if (brightness < 0) brightness = 0;
                    const char *color = get_color(final_normal);
                    char ch = brightness > 0.6 ? '█' :
                              brightness > 0.3 ? '▓' :
                              brightness > 0.1 ? '▒' : '░';

                    if (half == 0) {
                        top_char = ch;
                        top_color = color;
                    } else {
                        bottom_char = ch;
                        bottom_color = color;
                    }
                }
            }

            printf("%s%s▀\033[0m", top_color, bottom_color);
        }
        putchar('\n');
    }
}

int main() {
    Box cubes[NUM_CUBES];
    int idx = 0;
    float start = -1.0f;
    float size = CUBE_SIZE;
    float pad = SPACING;

    for (int x = 0; x < CUBES_PER_AXIS; x++) {
        for (int y = 0; y < CUBES_PER_AXIS; y++) {
            for (int z = 0; z < CUBES_PER_AXIS; z++) {
                float cx = start + x * (size + pad);
                float cy = start + y * (size + pad);
                float cz = 3.0f + z * (size + pad);
                Vec3 min = {cx - size/2, cy - size/2, cz - size/2};
                Vec3 max = {cx + size/2, cy + size/2, cz + size/2};
                cubes[idx++] = (Box){min, max};
            }
        }
    }

    Vec3 light_dir = vec_norm((Vec3){-1, -1, -1});

    for (int frame = 0; frame < 10000; frame++) {
        float angle = frame * 0.02f;
        Vec3 camera = {
            .x = 2.0f * sinf(angle),
            .y = 1.2f * sinf(angle * 0.5f),
            .z = -2.5f * cosf(angle)
        };

        printf("\033[H\033[J");
        render_frame(camera, cubes, NUM_CUBES, light_dir);
        usleep(100000);
    }

    return 0;
}
