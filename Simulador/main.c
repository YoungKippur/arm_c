// gcc main.c serial.c circulos.c angulos.c -lm -lallegro -lallegro_color -lallegro_primitives -I/usr/local/include -L/usr/local/lib -lwiringPi -o sim

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_color.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_primitives.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include "circulos.h"
#include "serial.h"
#include "angulos.h"

#define FPS 60.0
#define D_WIDTH_M  100
#define D_HEIGHT_M 200
#define RADIO_INF 200
#define RADIO_SUP 100


int main(int argc, char **argv) {
    if (init()){
        return -1;
    }

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    bool do_exit = false;
    bool modo = false;

    double punto_inf_x = (D_WIDTH_M + RADIO_INF + RADIO_SUP), punto_inf_x_m = (D_WIDTH_M + RADIO_INF + RADIO_SUP);
    double punto_inf_y = (D_HEIGHT_M + RADIO_INF + RADIO_SUP), punto_inf_y_m = (D_HEIGHT_M + RADIO_INF + RADIO_SUP);
    double punto_med_x = (D_WIDTH_M + RADIO_INF + RADIO_SUP), punto_med_x_m = (D_WIDTH_M + RADIO_INF + RADIO_SUP);
    double punto_med_y = (D_HEIGHT_M + RADIO_INF + RADIO_SUP) - RADIO_INF, punto_med_y_m = (D_HEIGHT_M + RADIO_INF + RADIO_SUP) - RADIO_INF;
    double punto_sup_x = (D_WIDTH_M + RADIO_INF + RADIO_SUP), punto_sup_x_m = (D_WIDTH_M + RADIO_INF + RADIO_SUP);
    double punto_sup_y = (D_HEIGHT_M + RADIO_INF + RADIO_SUP) - (RADIO_INF + RADIO_SUP), punto_sup_y_m = (D_HEIGHT_M + RADIO_INF + RADIO_SUP) - (RADIO_INF + RADIO_SUP);

    double m_pointer_x = 0;
    double m_pointer_y = 0;

    int pt_x = 0, pt_y = 0, est = 0;

    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }

    if (!al_install_mouse()) {
            fprintf(stderr, "failed to initialize the mouse!\n");
            return -1;
        }

    display = al_create_display((D_WIDTH_M + RADIO_INF + RADIO_SUP) * 2, (D_HEIGHT_M + RADIO_INF + RADIO_SUP));
    if (!display) {
        al_shutdown_primitives_addon();
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
    	fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        al_shutdown_primitives_addon();
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_clear_to_color(al_color_name("black"));
    al_flip_display(); // Estaba asi en el ejemplo

    al_clear_to_color(al_color_name("black"));

    al_flip_display();

    while (!do_exit){
    	ALLEGRO_EVENT ev;

        if (modo == 0){
            readd(&pt_x);
            readd(&pt_y);
            readd(&est);
            int ptt_x = pt_x * ((D_WIDTH_M + RADIO_INF + RADIO_SUP) * 2) / 4095;
            int ptt_y = pt_y * (D_HEIGHT_M + RADIO_INF + RADIO_SUP) / 4095;
            printf("x:%d     y:%d     %d\n", pt_x, pt_y, est);
            m_pointer_x = ptt_x;
            m_pointer_y = ptt_y;

            if (est == 1){
                char spos[6];
                if(calc_ang_sup(punto_inf_x, punto_inf_y, punto_med_x, punto_med_y, punto_sup_x, punto_sup_y, RADIO_INF, RADIO_SUP) != -1){ 
                    punto_inf_x_m = punto_inf_x;
                    punto_inf_y_m = punto_inf_y;
                    punto_med_x_m = punto_med_x;
                    punto_med_y_m = punto_med_y;
                    punto_sup_x_m = punto_sup_x;
                    punto_sup_y_m = punto_sup_y;
                    //printf("%d\n", calc_ang_sup(punto_inf_x_m, punto_inf_y_m, punto_med_x_m, punto_med_y_m, punto_sup_x, punto_sup_y, RADIO_INF, RADIO_SUP));
                }
                snprintf(spos, 6, "%d\n\n\0", 180-calc_ang_sup(punto_inf_x_m, punto_inf_y_m, punto_med_x_m, punto_med_y_m, punto_sup_x, punto_sup_y, RADIO_INF, RADIO_SUP));
                sendd(spos);
                snprintf(spos, 6, "%d\n\n\0", 180-calc_ang_inf(punto_inf_x_m, punto_inf_y_m, punto_med_x_m, punto_med_y_m), calc_ang_sup(punto_inf_x_m, punto_inf_y_m, punto_med_x_m, punto_med_y_m, punto_sup_x, punto_sup_y, RADIO_INF, RADIO_SUP));
                sendd(spos);

                while (est == 1){
                    readd(&pt_x);
                    readd(&pt_y);
                    readd(&est);
                }
            }
        }

    	if (al_get_next_event(event_queue, &ev)){
    		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
    			do_exit = true;
    		}
    		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                char spos[6];
                if(calc_ang_sup(punto_inf_x_m, punto_inf_y_m, punto_med_x_m, punto_med_y_m, punto_sup_x, punto_sup_y, RADIO_INF, RADIO_SUP) != -1){ 
                    punto_inf_x_m = punto_inf_x;
                    punto_inf_y_m = punto_inf_y;
                    punto_med_x_m = punto_med_x;
                    punto_med_y_m = punto_med_y;
                    punto_sup_x_m = punto_sup_x;
                    punto_sup_y_m = punto_sup_y;
                }
                snprintf(spos, 6, "%d\n\n\0", 180-calc_ang_sup(punto_inf_x_m, punto_inf_y_m, punto_med_x_m, punto_med_y_m, punto_sup_x, punto_sup_y, RADIO_INF, RADIO_SUP));
                sendd(spos);
                snprintf(spos, 6, "%d\n\n\0", 180-calc_ang_inf(punto_inf_x_m, punto_inf_y_m, punto_med_x_m, punto_med_y_m), calc_ang_sup(punto_inf_x_m, punto_inf_y_m, punto_med_x_m, punto_med_y_m, punto_sup_x, punto_sup_y, RADIO_INF, RADIO_SUP));
                sendd(spos);
                //printf("%d\n", calc_ang_sup(punto_inf_x_m, punto_inf_y_m, punto_med_x_m, punto_med_y_m, punto_sup_x, punto_sup_y, RADIO_INF, RADIO_SUP));
                
            }
    		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
    			if (modo == 1){
                    m_pointer_x = ev.mouse.x;
    			    m_pointer_y = ev.mouse.y;
                }
            }
    	}

    	if(circle_circle_intersection(punto_inf_x, punto_inf_y, RADIO_INF, m_pointer_x, m_pointer_y, RADIO_SUP, &punto_med_x, &punto_med_y)){
    			punto_sup_x = m_pointer_x;
    			punto_sup_y = m_pointer_y;
    	}

    	al_clear_to_color(al_color_name("black")); // Va todo en una funcion
    	al_draw_line(punto_inf_x_m, punto_inf_y_m, punto_med_x_m, punto_med_y_m, al_color_name("darkred"), 20.0);
    	al_draw_line(punto_med_x_m, punto_med_y_m, punto_sup_x_m, punto_sup_y_m, al_color_name("red"), 20.0);
    	al_draw_filled_circle(punto_inf_x_m, punto_inf_y_m, 10.0, al_color_name("lightgreen"));
    	al_draw_filled_circle(punto_med_x_m, punto_med_y_m, 10.0, al_color_name("lightgreen"));
    	al_draw_filled_circle(punto_sup_x_m, punto_sup_y_m, 10.0, al_color_name("lightgreen"));
    	// al_draw_circle(punto_inf_x, punto_inf_y, RADIO_INF+RADIO_SUP, al_color_name("lavenderblush"), 1.0);
    	// al_draw_circle(punto_med_x, punto_med_y, RADIO_SUP, al_color_name("lavenderblush"), 1.0);

    	al_draw_line(punto_inf_x, punto_inf_y, punto_med_x, punto_med_y, al_color_name("darkblue"), 4.0);
    	al_draw_line(punto_med_x, punto_med_y, punto_sup_x, punto_sup_y, al_color_name("blue"), 4.0);
    	al_draw_filled_circle(punto_inf_x, punto_inf_y, 3.0, al_color_name("hotpink"));
    	al_draw_filled_circle(punto_med_x, punto_med_y, 3.0, al_color_name("hotpink"));
    	al_draw_filled_circle(punto_sup_x, punto_sup_y, 3.0, al_color_name("hotpink"));

    	al_draw_filled_circle(m_pointer_x, m_pointer_y, 5.0, al_color_name("hotpink"));
        al_flip_display();
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_shutdown_primitives_addon();
    return 0;
}

