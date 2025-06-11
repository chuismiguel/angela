import tkinter as tk
from tkinter import messagebox, colorchooser
import requests
import yaml

# Load configuration from config.yaml
with open("config.yaml", "r") as config_file:
    config = yaml.safe_load(config_file)

server = "http://" + str(config['angela_ip']) + ":" + str(config['port']) + "/"

# Function to fetch sensor data from the Raspberry Pi server
def fetch_sensor_data():
    try:
        response = requests.get(server + "/ultrasensor")
        
        if response.status_code == 200:
            data = response.json()
            distance = data.get("distance")
            distance_label.config(text=f"Distance: {distance} m")
        else:
            messagebox.showerror("Error", "Failed to fetch sensor data")
    except requests.exceptions.RequestException as e:
        messagebox.showerror("Error", f"An error occurred: {e}")

# Function to open color picker and send the selected color to the Raspberry Pi server
def pick_and_send_color():
    color = colorchooser.askcolor(title="Choose LED Color")[0]  # Get RGB values (ignoring hex)
    
    if color:  # If a color was chosen
        rgb = [int(c) for c in color]  # Convert to integers
        
        # Send the RGB color to the Raspberry Pi server
        try:
            response = requests.post(
                server + "/led_color",
                json={"rgb": rgb}
            )
            
            if response.status_code == 200:
                messagebox.showinfo("Success", "LED color updated successfully")
            else:
                messagebox.showerror("Error", "Failed to update LED color")
        except requests.exceptions.RequestException as e:
            messagebox.showerror("Error", f"An error occurred: {e}")

def send_stop():
    # Send the X and Z values to the Raspberry Pi server
    try:
        response = requests.post(
            server + "/move",
            json={"x": 0, "z": 0}
        )
        
        if response.status_code == 200:
            messagebox.showinfo("Success", "Movement command sent successfully")
        else:
            messagebox.showerror("Error", "Failed to send movement command")
    except requests.exceptions.RequestException as e:
        messagebox.showerror("Error", f"An error occurred: {e}")

# Function to send X and Z values to the Raspberry Pi server
def send_movement_command():
    x_value = x_slider.get()
    z_value = z_slider.get()

    # Send the X and Z values to the Raspberry Pi server
    try:
        response = requests.post(
            server + "/move",
            json={"x": x_value, "z": z_value}
        )
        
        if response.status_code == 200:
            messagebox.showinfo("Success", "Movement command sent successfully")
        else:
            messagebox.showerror("Error", "Failed to send movement command")
    except requests.exceptions.RequestException as e:
        messagebox.showerror("Error", f"An error occurred: {e}")

# Create the main window
root = tk.Tk()
root.title("Sensor Data and LED Control")

# Label to display distance
distance_label = tk.Label(root, text="Distance: -- m", font=("Arial", 16))
distance_label.pack(pady=10)

# Button to fetch sensor data
fetch_button = tk.Button(root, text="Fetch Sensor Data", command=fetch_sensor_data, font=("Arial", 14))
fetch_button.pack(pady=20)

stop_button = tk.Button(root, text="Stop robot", command=send_stop, font=("Arial", 14))
stop_button.pack(pady=20)

# Button to open color picker and send selected color
color_button = tk.Button(root, text="Pick and Send LED Color", command=pick_and_send_color, font=("Arial", 14))
color_button.pack(pady=20)

# Create the X slider (range from -1 to 1)
x_slider_label = tk.Label(root, text="X Movement", font=("Arial", 14))
x_slider_label.pack(pady=10)
x_slider = tk.Scale(root, from_=-1, to=1, resolution=0.01, orient="horizontal", font=("Arial", 14))
x_slider.pack(pady=10)

# Create the Z slider (range from -1 to 1)
z_slider_label = tk.Label(root, text="Z Movement", font=("Arial", 14))
z_slider_label.pack(pady=10)
z_slider = tk.Scale(root, from_=-1, to=1, resolution=0.01, orient="horizontal", font=("Arial", 14))
z_slider.pack(pady=10)

# Button to send movement command based on slider values
move_button = tk.Button(root, text="Send Movement Command", command=send_movement_command, font=("Arial", 14))
move_button.pack(pady=20)

# Run the application
root.mainloop()
