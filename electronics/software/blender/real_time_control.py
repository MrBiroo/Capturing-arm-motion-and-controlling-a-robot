
import serial
import time
import bpy
import math
import threading

# Replace 'COM4' with your serial port and set the baud rate
serial_port = 'COM3'
baud_rate = 115200

# Names of objects you want to control
object_part4_name = 'Part4'  # Object to rotate for pitch
object_yaw_name = 'roll'   # Object to rotate for yaw
object_roll_name = 'sh'   # Object to rotate for roll

object_pitch_elb_name = 'el3'
object_roll_elb_name = 'el4'

object_pitch_wr_name = 'gearr1'
object_yaw_wr_name = 'hand2'

object_hori_finger_name = 'rc2'

object_rc4_name = 'rc4'
object_rc1_name = 'rc1'
object_rc3_name = 'rc3'

object_hori_finger1_name = 'rc2.001'

object_rc41_name = 'rc4.001'
object_rc11_name = 'rc1.001'
object_rc31_name = 'rc3.001'

object_hori_finger2_name = 'rc2.002'

object_rc42_name = 'rc4.002'
object_rc12_name = 'rc1.002'
object_rc32_name = 'rc3.002'

def update_objects(dest):
    # Find the target objects
    obj_part4 = bpy.data.objects.get(object_part4_name)
    obj_roll = bpy.data.objects.get(object_yaw_name)
    obj_sh = bpy.data.objects.get(object_roll_name)
    
    obj_el3= bpy.data.objects.get(object_pitch_elb_name)
    obj_el4= bpy.data.objects.get(object_roll_elb_name)
    
    obj_gear=bpy.data.objects.get(object_pitch_wr_name)
    obj_hand=bpy.data.objects.get(object_yaw_wr_name)
    
    obj_horiFinger=bpy.data.objects.get(object_hori_finger_name)
    obj_rc4=bpy.data.objects.get(object_rc4_name)
    obj_rc1=bpy.data.objects.get(object_rc1_name)
    obj_rc3=bpy.data.objects.get(object_rc3_name)
    
    obj_horiFinger1=bpy.data.objects.get(object_hori_finger1_name)
    obj_rc41=bpy.data.objects.get(object_rc41_name)
    obj_rc11=bpy.data.objects.get(object_rc11_name)
    obj_rc31=bpy.data.objects.get(object_rc31_name)
    
    obj_horiFinger2=bpy.data.objects.get(object_hori_finger2_name)
    obj_rc42=bpy.data.objects.get(object_rc42_name)
    obj_rc12=bpy.data.objects.get(object_rc12_name)
    obj_rc32=bpy.data.objects.get(object_rc32_name)

    pitch = 0
    yaw = 0
    roll = 0

    if dest.startswith('A'):  # For pitch, update Y-axis rotation
        pitch = float(dest[1:])  # Extract and convert to float
        obj_part4.rotation_euler[0] = math.radians(pitch)  # Set rotation around X (pitch)
        print("Pitch (X-axis): ", pitch)

    elif dest.startswith('B'):  # For yaw, update Z-axis rotation
        yaw = float(dest[1:])  # Extract and convert to float
        obj_roll.rotation_euler[1] = math.radians(yaw)  # Set rotation around Z (yaw)
        print("Yaw (Z-axis): ", yaw)

    elif dest.startswith('C'):
        roll = float(dest[1:])
        obj_sh.rotation_euler[1] = math.radians(roll)
        print("Roll (Y-axis): ", roll)
   
    elif dest.startswith('D'):  # For pitch, update Y-axis rotation
        pitch_elb = float(dest[1:])  # Extract and convert to float
        obj_el3.rotation_euler[0] = math.radians(pitch_elb)  # Set rotation around X (pitch)
        print("Pitch (X-axis): ", pitch_elb)
        
    elif dest.startswith('E'):
        roll_elb=float(dest[1:])
        obj_el4.rotation_euler[1] = math.radians(roll_elb)
        print("Roll (Y-axis): ",roll_elb)  
          
    elif dest.startswith('G'):  # For pitch, update Y-axis rotation
        pitch_wr = float(dest[1:])  # Extract and convert to float
        obj_gear.rotation_euler[0] = math.radians(pitch_wr)  # Set rotation around X (pitch)
        print("Pitch (X-axis): ", pitch_wr)
        
    elif dest.startswith('H'):  # For yaw, update Z-axis rotation
        yaw_wr = float(dest[1:])  # Extract and convert to float
        obj_hand.rotation_euler[2] = math.radians(yaw_wr)  # Set rotation around Z (yaw)
        print("Yaw (Z-axis): ", yaw_wr)
        
    elif dest.startswith('J'):  # For finger, update Z-axis rotation
        hori = float(dest[1:])  # Extract and convert to float
        obj_horiFinger.rotation_euler[2] = math.radians(hori)  # Set rotation around Z (yaw)
        print("Finger horizantal movment: ", hori)
     
    elif dest.startswith('K'):  # For finger, update Y-axis rotation
        finger = float(dest[1:])/-3  # Extract and convert to float
        obj_rc4.rotation_euler[0] = math.radians(finger)  # Set rotation around Z (yaw)
        obj_rc1.rotation_euler[0] = math.radians(finger)
        obj_rc3.rotation_euler[0] = math.radians(finger)
        print("Finger movment: ", finger)  

    elif dest.startswith('L'):  # For finger, update Z-axis rotation
        hori1 = float(dest[1:])  # Extract and convert to float
        obj_horiFinger1.rotation_euler[2] = math.radians(hori1)  # Set rotation around Z (yaw)
        print("Finger horizantal movment: ", hori)
     
    elif dest.startswith('M'):  # For finger, update Y-axis rotation
        finger1 = float(dest[1:])/-3  # Extract and convert to float
        obj_rc41.rotation_euler[0] = math.radians(finger1)  # Set rotation around Z (yaw)
        obj_rc11.rotation_euler[0] = math.radians(finger1)
        obj_rc31.rotation_euler[0] = math.radians(finger1)
        print("Finger movment: ", finger)   
             
    elif dest.startswith('N'):  # For finger, update Z-axis rotation
        hori2 = float(dest[1:])  # Extract and convert to float
        obj_horiFinger2.rotation_euler[2] = math.radians(hori2)  # Set rotation around Z (yaw)
        print("Finger horizantal movment: ", hori)
     
    elif dest.startswith('O'):  # For finger, update Y-axis rotation
        finger2 = float(dest[1:])/-3  # Extract and convert to float
        obj_rc42.rotation_euler[0] = math.radians(finger2)  # Set rotation around Z (yaw)
        obj_rc12.rotation_euler[0] = math.radians(finger2)
        obj_rc32.rotation_euler[0] = math.radians(finger2)
        print("Finger movment: ", finger)   
                     
    else:
        print("Invalid message type after '#': ", dest)

class SerialThread(threading.Thread):
    def __init__(self, port, rate):
        super().__init__()
        self.port = port
        self.rate = rate
        self.running = True

    def run(self):
        try:
            # Create a serial connection
            with serial.Serial(self.port, self.rate, timeout=1) as ser:
                print("Serial Port Opened: ", ser.name)
                time.sleep(2)  # Allow time to establish connection

                # Read data from the serial port
                while self.running:
                    if ser.in_waiting > 0:  # Check if there is data waiting
                        data = ser.readline().decode('utf-8').rstrip()  # Read a line
                        
                        # Process the data if it starts with '#'
                        if data.startswith('#'):
                            dest = data[1:]  # Remove the '#' character
                            update_objects(dest)

        except serial.SerialException as e:
            print("Error: ", e)

    def stop(self):
        self.running = False


# Global instance of the thread
serial_thread = SerialThread(serial_port, baud_rate)
serial_thread.start()

# Cleanup function when stopping the thread
def stop_serial_thread():
    serial_thread.stop()
    serial_thread.join()
    print("Serial Port Closed")

# To stop the serial thread, call stop_serial_thread()

