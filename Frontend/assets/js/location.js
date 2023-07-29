// JavaScript to show and close the location container

// Function to show the location container and display the map
function showLocation() {
    document.getElementById('locationContainer').classList.remove('hidden');
    // Call your function to display the channel location on the map here
    displayChannelLocationOnMap();
  }
  
  // Function to close the location container
  function closeLocation() {
    document.getElementById('locationContainer').classList.add('hidden');
  }
  