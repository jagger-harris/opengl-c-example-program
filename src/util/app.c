#include "app.h"
#include "camera.h"
#include "gfx/cube.h"
#include "gfx/quad.h"
#include "gfx/renderer.h"
#include "gfx/shader.h"
#include "gfx/texture.h"
#include "input.h"
#include "math/mat4.h"
#include "math/math.h"
#include "math/vec3.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>

app app_create(camera *camera, unsigned int width, unsigned int height,
               const char *title, bool vsync) {
  app app;

  app.window = window_create(width, height, title, vsync);
  app.camera = camera;
  app.width = width;
  app.height = height;
  app.title = title;
  app.vsync = vsync;

  return app;
}

void app_destroy(app *app) {
  window_destroy(app->window);
  camera_destroy(app->camera);
}

/**
 * @brief Main loop and general app functionalities
 *
 * @param app App which to run
 */
void app_run(app *app) {
  GLFWwindow *window = app->window;
  shader shader =
      shader_create("data/shaders/vertex.glsl", "data/shaders/fragment.glsl");
  quad quad = quad_create();
  cube cube = cube_create();
  texture container =
      texture_create("data/assets/images/container.jpg", GL_RGB);
  texture awesomeface =
      texture_create("data/assets/images/awesomeface.png", GL_RGBA);

  /* Renderer settings */
  renderer_settings();

  /* App loop */
  while (!glfwWindowShouldClose(window)) {
    /* Timing */
    float current_frame = glfwGetTime();
    app->delta_time = current_frame - app->last_frame;
    app->last_frame = current_frame;

    input_handle(app);

    renderer_clear(0.2f, 0.3f, 0.3f, 1.0f);

    texture_use(container, GL_TEXTURE0);
    texture_use(awesomeface, GL_TEXTURE1);
    shader_set_int(shader, "texture1", 0);
    shader_set_int(shader, "texture2", 1);
    renderer_use_shader(shader);

    /* Transformations */
    mat4 view = camera_lookat(app->camera);
    mat4 projection = mat4_perspective(
        45.0f, (float)app->width / (float)app->height, 0.1f, 100.0f);

    shader_set_mat4(shader, "view", &view);
    shader_set_mat4(shader, "projection", &projection);

    mat4 model = mat4_create_identity();
    model = mat4_translate(model, vec3_create_from_values(0.0f, 0.0f, 0.0f));
    shader_set_mat4(shader, "model", &model);
    cube_draw(cube);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
