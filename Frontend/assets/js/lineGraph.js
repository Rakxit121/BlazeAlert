    // Function to fetch data from ThingSpeak API
    async function fetchDataFromThingSpeak(channelId, numPoints) {
        const url = `https://api.thingspeak.com/channels/${channelId}/feeds.json?results=${numPoints}`;
        const response = await fetch(url);
        const data = await response.json();
        return data.feeds;
      }
  
      // Function to extract labels and data from fetched ThingSpeak data
      function extractDataFromThingSpeakData(feeds) {
        const labels = feeds.map((feed) => feed.created_at);
        const data = {
          humidity: feeds.map((feed) => parseFloat(feed.field1)),
          temperature: feeds.map((feed) => parseFloat(feed.field2)),
          "gas-level": feeds.map((feed) => parseFloat(feed.field3)),
          pressure: feeds.map((feed) => parseFloat(feed.field4)),
          altitude: feeds.map((feed) => parseFloat(feed.field5)),
        };
        return { labels, data };
      }
  
      // Function to create line chart for a specific field
      function createLineChart(canvasId, timestamps, fieldData, label, borderColor) {
        return new Chart(document.getElementById(canvasId).getContext("2d"), {
          type: "line",
          data: {
            labels: timestamps,
            datasets: [
              {
                label: label,
                data: fieldData,
                borderColor: borderColor,
                borderWidth: 2,
                fill: false,
              },
            ],
          },
          options: {
            responsive: true,
            scales: {
              x: {
                type: "time",
                time: {
                  unit: "minute",
                  displayFormats: {
                    minute: "HH:mm",
                  },
                },
                title: {
                  display: true,
                  text: "Time",
                },
              },
              y: {
                title: {
                  display: true,
                  text: "Value",
                },
              },
            },
          },
        });
      }
  
      // Function to create area chart for a specific field
      function createAreaChart(canvasId, timestamps, fieldData, label, backgroundColor) {
        return new Chart(document.getElementById(canvasId).getContext("2d"), {
          type: "line",
          data: {
            labels: timestamps,
            datasets: [
              {
                label: label,
                data: fieldData,
                borderColor: "transparent",
                backgroundColor: backgroundColor,
                borderWidth: 2,
                fill: true,
              },
            ],
          },
          options: {
            responsive: true,
            scales: {
              x: {
                type: "time",
                time: {
                  unit: "minute",
                  displayFormats: {
                    minute: "HH:mm",
                  },
                },
                title: {
                  display: true,
                  text: "Time",
                },
              },
              y: {
                title: {
                  display: true,
                  text: "Value",
                },
              },
            },
          },
        });
      }
  
      // Function to create bar chart for a specific field
      function createBarChart(canvasId, labels, fieldData, label, backgroundColor) {
        return new Chart(document.getElementById(canvasId).getContext("2d"), {
          type: "bar",
          data: {
            labels: labels,
            datasets: [
              {
                label: label,
                data: fieldData,
                backgroundColor: backgroundColor,
                borderWidth: 1,
              },
            ],
          },
          options: {
            responsive: true,
            scales: {
              x: {
                title: {
                  display: true,
                  text: "Time",
                },
              },
              y: {
                title: {
                  display: true,
                  text: "Value",
                },
              },
            },
          },
        });
      }
  
      // Function to create scatter chart for a specific field
      function createScatterChart(canvasId, fieldData, label, pointColor) {
        return new Chart(document.getElementById(canvasId).getContext("2d"), {
          type: "scatter",
          data: {
            datasets: [
              {
                label: label,
                data: fieldData,
                borderColor: pointColor,
                backgroundColor: pointColor,
                pointRadius: 5,
                pointHoverRadius: 8,
              },
            ],
          },
          options: {
            responsive: true,
            scales: {
              x: {
                title: {
                  display: true,
                  text: "Time",
                },
              },
              y: {
                title: {
                  display: true,
                  text: "Value",
                },
              },
            },
          },
        });
      }
  
     // Rendering initial charts
  document.addEventListener("DOMContentLoaded", async function () {
    const channelId = "2206646"; // Replace with your actual ThingSpeak channel ID
    const numPoints = 6; // Number of data points to retrieve

    try {
      // Fetch data from ThingSpeak API
      const feeds = await fetchDataFromThingSpeak(channelId, numPoints);

      // Extract labels and data from the fetched data
      const { labels, data } = extractDataFromThingSpeakData(feeds);

      // Create line charts for each field with updated labels
      var lineChart1 = createLineChart("lineChart1", labels, data.humidity, "Humidity", "#587ce4");
      var lineChart2 = createLineChart("lineChart2", labels, data.temperature, "Temperature", "#1D8348");
      var lineChart3 = createLineChart("lineChart3", labels, data["gas-level"], "Gas Level", "#FF5733");
      var lineChart4 = createLineChart("lineChart4", labels, data.pressure, "Pressure", "#FFA500");
      var lineChart5 = createLineChart("lineChart5", labels, data.altitude, "Altitude", "#800080");

      // Update the HTML elements with the fetched data
      const latestFeed = feeds[feeds.length - 1];
      document.getElementById("humidity").textContent = latestFeed.field1;
      document.getElementById("temperature").textContent = latestFeed.field2;
      document.getElementById("gas-level").textContent = latestFeed.field3;
      document.getElementById("pressure").textContent = latestFeed.field4;
      document.getElementById("altitude").textContent = latestFeed.field5;
    } catch (error) {
      console.error("Error fetching data:", error);
    }

    // Continuously update data every 5 seconds
    setInterval(async function () {
      const channelId = "2206646"; // Replace with your actual ThingSpeak channel ID
      const numPoints = 1; // Number of data points to retrieve (latest data)

      try {
        // Fetch latest data from ThingSpeak API
        const feeds = await fetchDataFromThingSpeak(channelId, numPoints);

        // Update the HTML elements with the latest data
        if (feeds.length > 0) {
          const latestFeed = feeds[0];
          document.getElementById("humidity").textContent = latestFeed.field1;
          document.getElementById("temperature").textContent = latestFeed.field2;
          document.getElementById("gas-level").textContent = latestFeed.field3;
          document.getElementById("pressure").textContent = latestFeed.field4;
          document.getElementById("altitude").textContent = latestFeed.field5;
        }
      } catch (error) {
        console.error("Error fetching data:", error);
      }
    }, 5000); // Update every 5 seconds
  });