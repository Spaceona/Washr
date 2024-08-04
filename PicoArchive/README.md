# Hackathon_Washr
# Hackathon_Washr
Ruef Laundry Room Monitoring System
This web application monitors the status of a washing machine in the Ruef Laundry Room using data from a Raspberry Pi Pico with an attached MPU6050 gyroscope module. The application fetches real-time data from a Firebase Realtime Database and displays the status of "Washing Machine 1" in a color-coded box.

Setup
Firebase Configuration:

Replace the placeholder values in the script with your Firebase project configuration.

javascript
Copy code
const firebaseConfig = {
    apiKey: "YOUR_API_KEY",
    authDomain: "YOUR_AUTH_DOMAIN",
    projectId: "YOUR_PROJECT_ID",
    databaseURL: "YOUR_DATABASE_URL",
    storageBucket: "YOUR_STORAGE_BUCKET",
    messagingSenderId: "YOUR_MESSAGING_SENDER_ID",
    appId: "YOUR_APP_ID"
};
Firebase Realtime Database:

Set up a Firebase Realtime Database and configure the rules appropriately.
HTML and JavaScript:

Copy and paste the provided HTML and JavaScript code into your project.
Dependencies:

Ensure that you have the necessary Firebase JavaScript SDK versions included. (As of the provided code, it uses version 9.1.0)

Usage
Open the HTML file in a web browser.

Features
Displays the status of "Washing Machine 1" in a color-coded box.
Real-time updates every 15 seconds.

Troubleshooting
If the box color is not updating correctly, check the console in the browser's developer tools for error messages.

Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

License
This project is licensed under the MIT License - see the LICENSE file for details.

# Washr
# Washr
