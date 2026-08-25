 #include <crow.h>
 #include "controllers/sensor_controller.cpp"

 int main() {
      crow::SimpleApp app;

      // Routes will be defined in sensor_controller.cpp
      CROW_ROUTE(app, "/")([](){
          return "Sensor Service is running";
      });

      app.port(8080)
         .multithreaded()
         .run();

      return 0;
 }