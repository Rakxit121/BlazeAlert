import torch
import torchvision.transforms as transforms
from PIL import Image
import matplotlib.pyplot as plt
import torch.nn as nn
import os
from time import sleep
# Set device
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")



# Define your model class
#define architecture for our baseline CNN model
class Baseline(nn.Module):

    def __init__(self, input_shape: int, hidden_units: int, output_shape: int) -> None:
        super().__init__()
        
        self.conv_block_1 = nn.Sequential(
            #first convolution layer
            nn.Conv2d(in_channels=input_shape, out_channels=hidden_units, kernel_size=3, stride=1, padding=1),
            nn.ReLU(),
            #second convolution layer
            nn.Conv2d(in_channels=hidden_units, out_channels=hidden_units, kernel_size=3, stride=1, padding=1),
            nn.ReLU(),
            #pooling layer
            nn.MaxPool2d(kernel_size=3, stride=3)
        )

        self.classifier = nn.Sequential(
            nn.Flatten(),
            #we can find the number of layers checking the summary from torchinfo (example below)
            nn.Linear(in_features=17640,
                      out_features=output_shape)
        )
    
    def forward(self, x: torch.Tensor):
        
        return self.classifier(self.conv_block_1(x)) #leverage the benefits of operator fusion


# Define your train function

# Define your prediction function
#define function to predict custom images
def pred_and_plot_image(model, image_path, transform, device=torch.device('cuda' if torch.cuda.is_available() else 'cpu')):
    # Load the image
    img = Image.open(image_path)

    image_transform = transform

    model.to(device)
    model.eval()
    with torch.inference_mode():
        transformed_image = image_transform(img).unsqueeze(dim=0)
        target_image_pred = model(transformed_image.to(device))

        target_image_pred_probs = torch.sigmoid(target_image_pred)
        target_image_pred_label = torch.round(target_image_pred_probs)

    # Determine the class label
    class_label = "NOTFIRE" if target_image_pred_label.cpu().numpy()[0][0] == 1 else "FIRE"
    probability = target_image_pred_probs.cpu().numpy()[0][0]

   
    return class_label


# Define the path to your trained model's .pth file
model_path = "best_baseline_model.pth"

# Load the model
model = Baseline(input_shape=3,
                  hidden_units=10, 
                  output_shape=1)
model.load_state_dict(torch.load(model_path))
model.to(device)

# Define the image path you want to predict on
image_path = "OIP.jpg"

# Define the transformation for the image
transform = transforms.Compose([
    transforms.Resize(size=(128, 128)),             #Resizing the image to 128x128
    transforms.TrivialAugmentWide(num_magnitude_bins=31),       #TrivialAugmentWide is a transformation that randomly applies one of the following transformations to an image: AutoContrast, Brightness, Color, Contrast, Equalize, Invert, Posterize, Rotate, Sharpness, ShearX, ShearY, Solarize, TranslateX, TranslateY
    #turn the image into a torch.Tensor and also converts all pixel values from 0 to 255 to be between 0.0 and 1.0 
    transforms.ToTensor() 
])




def check_pred(fire_counter, non_fire_counter):
    # Define the image path you want to predict on
    image_path = "photo.jpg"

    # Check if the image file exists
    if os.path.exists(image_path):
        # Call the prediction function
        prediction = pred_and_plot_image(model, image_path, transform, device)
        if prediction == "FIRE":	
            fire_counter += 1
        elif prediction == "NOTFIRE":
            non_fire_counter += 1
    
        
    
        
    return fire_counter, non_fire_counter


while True:
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
    
    # Wait for some time before checking again
    sleep(1)