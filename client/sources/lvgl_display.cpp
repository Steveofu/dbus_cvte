// #include "lvgl.h"
// #include <SDL2/SDL.h>

// #define DISP_WIDTH 240
// #define DISP_HEIGHT 320

// static lv_disp_buf_t disp_buf;
// static lv_color_t buf[DISP_WIDTH * 10]; // 为屏幕缓冲区分配内存

// SDL_Window *window;
// SDL_Renderer *renderer;
// SDL_Texture *texture;

// void lvgl_init() {
//     lv_init();

//     // 初始化 SDL
//     if (SDL_Init(SDL_INIT_VIDEO) != 0) {
//         printf("SDL_Init Error: %s\n", SDL_GetError());
//         return;
//     }

//     // 创建 SDL 窗口
//     window = SDL_CreateWindow("LVGL Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISP_WIDTH, DISP_HEIGHT, SDL_WINDOW_SHOWN);
//     if (!window) {
//         printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
//         return;
//     }

//     // 创建 SDL 渲染器
//     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     if (!renderer) {
//         printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
//         return;
//     }

//     // 创建 SDL 纹理
//     texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, DISP_WIDTH, DISP_HEIGHT);
//     if (!texture) {
//         printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
//         return;
//     }

//     // 初始化显示缓冲区
//     lv_disp_buf_init(&disp_buf, buf, NULL, DISP_WIDTH * 10);

//     // 创建 LVGL 显示驱动
//     lv_disp_drv_t disp_drv;
//     lv_disp_drv_init(&disp_drv);
//     disp_drv.hor_res = DISP_WIDTH;
//     disp_drv.ver_res = DISP_HEIGHT;
//     disp_drv.buffer = &disp_buf;
//     disp_drv.flush_cb = sdl_flush_cb;

//     lv_disp_drv_register(&disp_drv);
// }

// // SDL 显示刷新回调
// void sdl_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {
//     // 更新 SDL 纹理
//     SDL_UpdateTexture(texture, NULL, color_p, DISP_WIDTH * sizeof(lv_color_t));

//     // 渲染
//     SDL_RenderClear(renderer);
//     SDL_RenderCopy(renderer, texture, NULL, NULL);
//     SDL_RenderPresent(renderer);

//     // 通知 LVGL 显示已更新
//     lv_disp_flush_ready(disp_drv);
// }

// void lvgl_loop() {
//     // 任务循环，处理 LVGL 任务
//     while (true) {
//         lv_task_handler();  // 处理 LVGL 任务
//         SDL_Delay(5);       // 延时，避免 CPU 占用过高
//     }
// }
