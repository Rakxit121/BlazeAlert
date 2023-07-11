import math
from djitellopy import Tello

# Function to handle the drone actions at a specific location
def perform_location_actions(drone, location):
    print(f"Performing actions at {location}")

    # Move backward 20 cm
    drone.move_back(20)

    drone.move_left(20)
    drone.move_forward(20)
    drone.rotate_clockwise(90)
    drone.move_left(20)
    drone.move_forward(20)
    drone.rotate_clockwise(90)
    drone.move_left(20)
    drone.move_forward(20)
    drone.rotate_clockwise(90)
    drone.move_left(20)
    drone.move_forward(20)
    drone.rotate_clockwise(90)
    
        

    print("Actions completed!")

# Main function
def main():
    drone = Tello()
    drone.connect()

    # Take off and go to home location
    drone.takeoff()
    print("Drone took off!")

    # Get user input for the desired location coordinates
    x = int(input("Enter the x-coordinate: "))
    y = int(input("Enter the y-coordinate: "))
    z = 20

    # Go to the specified location
    print(f"Going to location ({x}, {y}, {z})...")
    drone.go_xyz_speed(x, y, z, 10)  # Adjust the speed as per your needs

    # Perform actions at the location
    perform_location_actions(drone, f"({x}, {y}, {z})")

    print("Returning to home...")
    drone.go_xyz_speed(30, 20, 20, 10)  # Return to the initial location
    drone.land()

# Execute the main function
if __name__ == "__main__":
    main()
