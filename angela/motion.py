


from adafruit_motor import servo
from adafruit_pca9685 import PCA9685
import board

def pwm_to_hex(speed):
    # Map speed to the range [0, 1], where:
    # speed = 0 corresponds to 0x7FFF
    # speed = 1 corresponds to 0xFFFF
    dat = int(speed * float(0xFFFF))
    if dat > 0xFFFF:
        dat = 0xFFFF
    elif dat < 0x7FFF:
        dat = 0x7fff
    return dat

def get_motor_output_from_cmd(x, z):
    # with only linear vel considered, pwm equals value

    if (x >= 0):
        pwm_a = x
        pwm_b = 0.0
        pwm_c = x
        pwm_d = 0.0
    else :
        pwm_a = 0.0
        pwm_b = -1 * x
        pwm_c = 0.0
        pwm_d = -1 * x

    if (-1.0 <= z <= -0.5):
        pwm_a = 0.0
        pwm_b = pwm_b + (abs(z) - 0.5) * 2.0

    elif (-0.5 < z <= 0.0):
        pwm_a = pwm_a - (abs(z) * 2)
        pwm_b = 0.0
    elif ( 0.0 < z <= 0.5):
        pwm_c = pwm_c - (abs(z) * 2)
        pwm_d = 0.0
    else :
        pwm_c = 0.0
        pwm_d = pwm_d + (abs(z) - 0.5) * 2.0

    print(f"Result pwms: {pwm_a} {pwm_b} {pwm_c} {pwm_d}")
    # modify the given PWM cycles to match for turns:
    # Return duty cycles as integers
    return pwm_to_hex(pwm_a), pwm_to_hex(pwm_b), pwm_to_hex(pwm_c), pwm_to_hex(pwm_d)



class AngelaMovement:
    def __init__(self, head_idx, left_arm_idx, right_arm_idx, motor_a_fwd_idx, motor_a_bwd_idx, motor_b_fwd_idx , motor_b_bwd_idx): 
        
        i2c = board.I2C()  # uses board.SCL and board.SDA
        # i2c = busio.I2C(board.GP1, board.GP0)    # Pi Pico RP2040

        # Create a simple PCA9685 class instance.
        self.board = PCA9685(i2c)
        self.board.frequency = 60

        # servos
        
        self.left_arm = servo.Servo(self.board.channels[left_arm_idx])
        self.right_arm = servo.Servo(self.board.channels[right_arm_idx])
        self.head = servo.Servo(self.board.channels[head_idx])

        #motors
        self.left_motor_fwd = self.board.channels[motor_a_fwd_idx]
        self.left_motor_bwd = self.board.channels[motor_a_bwd_idx]
        self.right_motor_fwd = self.board.channels[motor_b_fwd_idx]
        self.right_motor_bwd = self.board.channels[motor_b_bwd_idx]
        
    def move(self,linearVel, angularVel):
        
        if linearVel == 0 and angularVel == 0:
            self.left_motor_fwd.duty_cycle =  0x7fff
            self.left_motor_bwd.duty_cycle =  0x7fff
            self.right_motor_fwd.duty_cycle =  0x7fff
            self.right_motor_bwd.duty_cycle =  0x7fff
            return
        angles = get_motor_output_from_cmd(linearVel, angularVel)
        self.left_motor_fwd.duty_cycle = angles[0]
        self.left_motor_bwd.duty_cycle = angles[1]
        self.right_motor_fwd.duty_cycle = angles[2]
        self.right_motor_bwd.duty_cycle = angles[3]
