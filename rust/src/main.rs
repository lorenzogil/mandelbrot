extern crate sdl2;

use sdl2::pixels::Color;
use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::rect::Point;
use sdl2::render::Renderer;

const SCREEN_WIDTH: u32 = 600;
const SCREEN_HEIGHT: u32 = 600;
const MAX_ITERATIONS: usize = 128;

fn gradient (pos: f64) -> Color {
    match pos {
        0.0 ... 0.16 => {
            let ratio = (pos - 0.0) / (0.16 - 0.0);
            Color::RGB(
                (0.0 + ratio * (32.0 - 0.0)) as u8,
                (7.0 + ratio * (107.0 - 7.0)) as u8,
                (100.0 + ratio * (203.0 - 100.0)) as u8
            )
        },
        0.16 ... 0.42 => {
            let ratio = (pos - 0.16) / (0.42 - 0.16);
            Color::RGB(
                (32.0 + ratio * (237.0 - 32.0)) as u8,
                (107.0 + ratio * (255.0 - 107.0)) as u8,
                (203.0 + ratio * (255.0 - 203.0)) as u8
            )
        },
        0.42 ... 0.6425 => {
            let ratio = (pos - 0.42) / (0.6425 - 0.42);
            Color::RGB(
                (237.0 + ratio * (255.0 - 237.0)) as u8,
                (255.0 + ratio * (170.0 - 255.0)) as u8,
                (255.0 + ratio * (0.0 - 255.0)) as u8
            )
        },
        0.6425 ... 0.8575 => {
            let ratio = (pos - 0.6425) / (0.8575 - 0.6425);
            Color::RGB(
                (255.0 + ratio * (0.0 - 255.0)) as u8,
                (170.0 + ratio * (2.0 - 170.0)) as u8,
                0 as u8
            )
        },
        _ => {
            let ratio = (pos - 0.8575) / (1.0 - 0.8575);
            Color::RGB(
                0 as u8,
                (2.0 + ratio * (7.0 - 2.0)) as u8,
                (0.0 + ratio *(100.0 - 0.0)) as u8
            )
        }
    }
}

fn render_mandelbrot(renderer: &mut Renderer, colors: [Color; MAX_ITERATIONS], viewport_size: f64, center_x: f64, center_y: f64) {
    let mut index;
    let (mut x, mut y);
    let (mut z, mut zi, mut newz, mut newzi);
    let mut smooth;

    for j in 0..SCREEN_HEIGHT {
        for i in 0..SCREEN_WIDTH {
            // transform pixel coordinates to viewport coordinates
            x = center_x - 0.5 * viewport_size + ((i as f64) / (SCREEN_WIDTH as f64)) * viewport_size;
            y = center_y - 0.5 * viewport_size + ((j as f64) / (SCREEN_HEIGHT as f64)) * viewport_size;

            // compute the Mandelbrot formula
            index = 0;
            z = 0.0;
            zi = 0.0;
            while index < MAX_ITERATIONS {
                newz = (z * z) - (zi * zi) + x;
                newzi = 2.0 * z * zi + y;
                z = newz;
                zi = newzi;
                if ((z * z) + (zi * zi)) > 4.0 {
                    break;
                }
                index = index + 1;
            }

            if index == 0 || index >= MAX_ITERATIONS {
                renderer.set_draw_color(Color::RGB(0, 0, 0));
            } else {
                smooth = (index as f64) + 1.0 - (z * z + zi * zi).sqrt().log(2.0).log(2.0) / 2.0_f64.log(2.0);
                renderer.set_draw_color(colors[smooth as usize]);
            }
            renderer.draw_point(Point::new(i as i32, j as i32)).unwrap();
        }
    }
    renderer.present();
}


fn main () {
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();

    let window = video_subsystem.window("Mandelbrot", SCREEN_WIDTH, SCREEN_HEIGHT)
        .position_centered()
        .opengl()
        .build()
        .unwrap();

    let mut renderer = window.renderer().build().unwrap();

    let mut colors = [Color::RGB(0, 0, 0); MAX_ITERATIONS];
    for i in 0..MAX_ITERATIONS {
        colors[i] = gradient((i as f64) / (MAX_ITERATIONS as f64));
    }

    let (x, y) = (0.0_f64, 0.0_f64);
    let viewport_size = 4.0_f64;
    let mut event_pump = sdl_context.event_pump().unwrap();

    'running: loop {
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit {..} | Event::KeyDown { keycode: Some(Keycode::Escape), .. } => {
                    break 'running
                },
                _ => {}
            }
        }
        render_mandelbrot(&mut renderer, colors, viewport_size, x, y);
    }
}
