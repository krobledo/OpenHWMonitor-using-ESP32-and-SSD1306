import serial
import time
import wmi

# Open Hardware Monitor API
w = wmi.WMI(namespace=r"root\OpenHardwareMonitor")

def get_sensor_data():
    data = {}
    temperature_infos = w.Sensor()
    for sensor in temperature_infos:
        if sensor.SensorType == 'Temperature':
            if sensor.Name == 'CPU Package':
                data['cpu_temp'] = f"{sensor.Value:.1f}"  # Limit to 4 digits
            elif 'GPU' in sensor.Name:
                data['gpu_temp'] = f"{sensor.Value:.1f}"  # Limit to 4 digits
        elif sensor.SensorType == 'Load':
            if sensor.Name == 'CPU Total':
                data['cpu_load'] = f"{sensor.Value:.1f}"  # Limit to 4 digits
            elif sensor.Name == 'Memory':
                data['memory_load'] = f"{sensor.Value:.1f}"  # Limit to 4 digits
            elif sensor.Name == 'GPU Core':
                data['gpu_core'] = f"{sensor.Value:.1f}"  # Limit to 4 digits
    return data

def format_data(data):
    return (f"CPU Temp: {data.get('cpu_temp', 'N/A')} C\n"
            f"GPU Temp: {data.get('gpu_temp', 'N/A')} C\n"
            f"GPU Core: {data.get('gpu_core', 'N/A')} %\n"
            f"Memory Load: {data.get('memory_load', 'N/A')} %\n")

ser = serial.Serial('COM3', 115200)  # Replace 'COM3' with your ESP32's serial port

try:
    while True:
        sensor_data = get_sensor_data()
        formatted_data = format_data(sensor_data)
        ser.write(formatted_data.encode())
        print(f"Sent data:\n{formatted_data}")  # Debugging print
        time.sleep(2)  # Update every 2 seconds
except KeyboardInterrupt:
    ser.close()
except serial.SerialException as e:
    print(f"Serial error: {e}")
