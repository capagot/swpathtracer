#include "main.h"

int main(int num_args, char** arg_vector) {
    SwpathtracerApp app(num_args, arg_vector);

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
