#include <unistd.h>
#include "vehicle/bus.h"
#include "log/log.h"
#include <time.h>

/*******************
 *
 * Goal: Print mean value (based on the last 10 values) of engine
 * speed (approx.) every second.
 *
 * We'll break this goal down to three smaller tasks, and these into
 * even smaller sub tasks. The sub tasks are close to pseudo code and
 * should, given some thoughts, be possible to translate in to C code
 * - sometimes using one line of code and sometimes using a few lines.
 *
 * You need to find a good place to do place the C code. We will give
 * some hint though.
 *
 * Tasks:
 *
 * 1) Store the value (either the entire struct or just the engine
 *    speed) in an array - store only the last 10 values/structs
 *    1.1) declare an array
 *    1.2) make sure the data is valid (check the return value)
 *    1.3) if not 10 items, store this item
 *         if 10 items, discard the first and store the new item
 *
 * 2) Check if 1 sec has passed since last print out
 *    2.1) declare variables previous_time and current_time (find a suitable type)
 *    2.2) check if previous_time - current_time > 1 sec 
 *    2.3) if the above (2.2) is true store current_time in previous_time
 * 
 * 3) Calc the mean value 
 *    3.1) If one sec has passed (see (2)), calculate mean value of 10
 *         last values of engine_speed and print out the mean value
 *    3.2) If one sec has not passed, do nothing
 *
 ***************************/
 
void setup_variables(unsigned long *current_time, unsigned long *previous_time, unsigned int *array);
int handle_bus_data(vehicle_info *vi, int index, unsigned int *array);
int handle_mean(unsigned int *array, unsigned long *current_time, unsigned long *previous_time);

int main() {
    vehicle_info vi;
    unsigned int meanArray[10];
    int index = 0;
    unsigned long current_time;
    unsigned long previous_time;
    setup_variables(&current_time, &previous_time, meanArray);

    while (1) {
        /*  Debug printout - only seen with
        *   make clean debug 
        */
        debug(("Getting new data\n"));
        index = handle_bus_data(&vi, index, meanArray);

        /* Log the data - only seen with either
        *   make clean log
        *   make clean debug 
        */
        log_vehicle_info(&vi);
        handle_mean(meanArray, &current_time, &previous_time);

        /* Sleep for 100 useconds, that is 1/10 of second */
        usleep(1000*100);
    }

    return 0;
}

void setup_variables(unsigned long *current_time, unsigned long *previous_time, unsigned int *array) {
    for (int i = 0; i < 10; ++i) {
        array[i] = 0;
    }

    *current_time = time(NULL);
    *previous_time = *current_time;
}

int handle_bus_data(vehicle_info *vi, int index, unsigned int *array) {
    int status;
    status = get_bus_data(vi);

    if (status == 0) {
        array[index++] = vi->engine_speed;
        if (index >= 10) {
            index = 0;
        }
    }
    return index;
}

int handle_mean(unsigned int *array, unsigned long *current_time, unsigned long *previous_time) {
    int mean_value = 0;
    *current_time = time(NULL);

    if (*current_time - *previous_time >= 1) {
        if (array[9] != 0) {
            for (int i = 0; i < 10; ++i)
            {
                mean_value += array[i];
            }
            mean_value /= 10;
            log(("mean value:     %d\n", mean_value));
        }
        *previous_time = *current_time;
    }
    return 0;
}
