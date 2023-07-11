import requests
import time
import json
import pickle
import pandas as pd
import joblib
from sklearn.ensemble import RandomForestClassifier
from model_evaluation import check_pred



def retrieve_new_values(api_key, channel_id):
    last_entry_id = None

    while True:
        # Make a request to the ThinkSpeak API
        response = requests.get(f'https://api.thingspeak.com/channels/{channel_id}/feeds.json?api_key={api_key}&results=1')

        if response.status_code == 200:
            data = response.json()

            # Retrieve the latest entry
            entry = data['feeds'][0]
            entry_id = entry['entry_id']

            # Check if a new entry is added
            if entry_id != last_entry_id:
                # Create a JSON combining the field data
                field_data = {
                    "Humidity": entry['field1'],
                    "Temperature": entry['field2'],
                    "Gas Level": entry['field3'],
                    "Pressure": float(float(entry['field4'])/100),
                    "Altitude": entry['field5']
                }
                json_data = json.dumps(field_data)
                print(json_data)
                new_data_received(json_data)  # Call new_data_received function with the JSON data
                last_entry_id = entry_id
                

    


def new_data_received(json_data):
    # Load the preprocessed model
    with open('random_forest_model.pkl', 'rb') as file:
        model = pickle.load(file)
    
    # Check if json_data is a string and convert it to a dictionary
    if isinstance(json_data, str):
        json_data = json.loads(json_data)

    # Check if json_data is a dictionary
    if isinstance(json_data, dict):
        # Convert string values to appropriate data types
        modified_data = {
            "inputs": {
                "Temperature": [float(json_data["Temperature"])],
                "Humidity": [float(json_data["Humidity"])],
                "Gas Level": [float(json_data["Gas Level"])],
                "Pressure": [float(json_data["Pressure"])],
                "Altitude": [float(json_data["Altitude"])]
            }
        }
        
        

        # Create a DataFrame from the modified JSON data
        new_data_df = pd.DataFrame(modified_data['inputs'])
                
        # Perform prediction using the preprocessed model
        prediction = model.predict(new_data_df)
        
        
        # Check if the prediction indicates fire (1) or not (0)
        if prediction[0] == "Yes":
            probablefire()
        else:
            noprobablefire()
    else:
        print("Invalid JSON data format. Expected dictionary or string.")


def send_drone():
    # Send a drone to the location of the fire
    print("Sending drone to the location of the fire.")

def probablefire():
    # Perform actions when a fire is probable
    send_drone()
    
    
    fire_counter = 0
    non_fire_counter = 0
    
    for i in range(20):
        fire_counter,non_fire_counter = check_pred(fire_counter, non_fire_counter)
    
    if fire_counter > non_fire_counter:
        print("ALERT THIS IS A FIRE")
    elif fire_counter < non_fire_counter:
        print("This is not a fire")
    else:
        print("Image file not found.")
    
    
    
    
    
def noprobablefire():
    # Perform actions when a fire is not probable
    print("Fire is not probable. No need to take any action.")
    



# Set your ThinkSpeak API key and channel ID
api_key = 'U7GLNV6TVILHOZST'
channel_id = '2206646'


# Call the function to retrieve new values
retrieve_new_values(api_key, channel_id)
#new_data_received(trial_data)
