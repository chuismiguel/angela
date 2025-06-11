import yaml
from flask import Flask, request, jsonify
import random  # For generating example sensor data

from ultrasensor import Ultrasound, SonarLeds
from motion import AngelaMovement

# Load configuration from config.yaml
with open("config.yaml", "r") as config_file:
    config = yaml.safe_load(config_file)


class AngelaServer:
    def __init__(self):
        self.app = Flask(__name__)
        self.ultrasensor = Ultrasound(pin = config["ultrasensor"]["gpio"], max_distance = config["ultrasensor"]["max_read_distance"])
        self.leds = SonarLeds(pin = config["leds"]["gpio"])
        self.movement = AngelaMovement(head_idx = config["servos"]["head"]["index"],
                                       left_arm_idx = config["servos"]["arm"]["left_arm_index"], 
                                       right_arm_idx = config["servos"]["arm"]["right_arm_index"], 
                                       motor_a_fwd_idx = config["motors"]["left_forward_index"],
                                       motor_a_bwd_idx = config["motors"]["left_backward_index"],
                                       motor_b_fwd_idx = config["motors"]["right_forward_index"],
                                       motor_b_bwd_idx = config["motors"]["right_backward_index"]
                                       )
        
        self.setup_sensor_routes()
        self.setup_actuator_routes()

    def run(self):
        self.app.run(host='0.0.0.0', port=config['server']['port'])
    
    def get_ultrasensor_distance(self):
        distance = self.ultrasensor.get_distance()
        
        if distance is None:
            return jsonify({"status": "error", "message": "Sensor error or out of range"}), 400

        sensor_data = {
            'distance': round(distance, 2)  # The actual distance reading from the ultrasonic sensor
        }
        return jsonify(sensor_data), 200
    
    def set_led_color(self):
        data = request.get_json()
        if data and 'rgb' in data:
            rgb = data['rgb']
            
            # Check if RGB is a valid list of 3 integers within the range 0-255
            if (isinstance(rgb, list) and len(rgb) == 3 and 
                    all(isinstance(x, int) and 0 <= x <= 255 for x in rgb)):

                self.leds.color((rgb[0], rgb[1], rgb[2]))
                return jsonify({"status": "success", "message": "LED color set"}), 200
            else:
                return jsonify({"status": "error", "message": "Invalid RGB format"}), 400

    def move(self):
        data = request.get_json()
        print("received data")
        if data and 'x' in data and 'z' in data:
            x = data['x']
            z = data['z']

            print(f"received {x} {z}")
            # Check if x and z are valid floats within the range -1 to 1
            if (isinstance(x, (int, float)) and -1 <= x <= 1 and
                isinstance(z, (int, float)) and -1 <= z <= 1):

                # Get motor outputs from the x and z values
                self.movement.move(x,z)

                return jsonify({
                    "status": "success",
                    "message": "Motors moved"
                }), 200
            else:
                return jsonify({"status": "error", "message": "Invalid x or z value"}), 400
        else:
            return jsonify({"status": "error", "message": "x and z values are required"}), 400
                
    def setup_sensor_routes(self):
        self.app.add_url_rule('/ultrasensor', 'get_ultrasensor_distance', self.get_ultrasensor_distance, methods=['GET'])
   
    def setup_actuator_routes(self):
            self.app.add_url_rule('/led_color', 'set_led_color', self.set_led_color, methods=['POST'])
            self.app.add_url_rule('/move', 'move', self.move, methods=['POST'])

if __name__ == '__main__':
    server = AngelaServer()
    server.run()
