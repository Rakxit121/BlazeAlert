document.addEventListener("DOMContentLoaded", function() {
    // Get references to tab elements and content sections
    const tabSensor1 = document.getElementById("tabSensor1");
    const tabSensor2 = document.getElementById("tabSensor2");
    const contentSensor1 = document.getElementById("contentSensor1");
    const contentSensor2 = document.getElementById("contentSensor2");

    // Hide content for Sensor 2 by default
    contentSensor2.style.display = "none";

    // Event listener for tab clicks
    tabSensor1.addEventListener("click", function() {
        tabSensor1.classList.add("active");
        tabSensor2.classList.remove("active");
        contentSensor1.style.display = "block";
        contentSensor2.style.display = "none";
    });

    tabSensor2.addEventListener("click", function() {
        tabSensor2.classList.add("active");
        tabSensor1.classList.remove("active");
        contentSensor2.style.display = "block";
        contentSensor1.style.display = "none";
    });
});