#!/usr/bin/env php
<?php

// Simple weather data simulation
$weather_conditions = [
    'Sunny' => ['icon' => '☀️', 'temp' => rand(25, 35), 'desc' => 'Clear skies'],
    'Cloudy' => ['icon' => '☁️', 'temp' => rand(18, 25), 'desc' => 'Overcast'],
    'Rainy' => ['icon' => '🌧️', 'temp' => rand(15, 22), 'desc' => 'Light rain'],
    'Partly Cloudy' => ['icon' => '⛅', 'temp' => rand(20, 28), 'desc' => 'Mixed conditions'],
    'Stormy' => ['icon' => '⛈️', 'temp' => rand(16, 23), 'desc' => 'Thunderstorms']
];

$current_condition = array_rand($weather_conditions);
$weather_data = $weather_conditions[$current_condition];
$current_time = date('H:i');
$current_date = date('M j, Y');

$body = <<<HTML
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Weather Widget</title>
    <style>
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: linear-gradient(135deg, #74b9ff, #0984e3);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            margin: 0;
            padding: 20px;
        }
        
        .weather-card {
            background: rgba(255, 255, 255, 0.9);
            backdrop-filter: blur(10px);
            border-radius: 20px;
            padding: 40px;
            text-align: center;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
            max-width: 300px;
            width: 100%;
        }
        
        .weather-icon {
            font-size: 4em;
            margin-bottom: 20px;
            display: block;
        }
        
        .temperature {
            font-size: 3em;
            font-weight: bold;
            color: #2d3436;
            margin: 0;
        }
        
        .condition {
            font-size: 1.3em;
            color: #636e72;
            margin: 10px 0;
            font-weight: 500;
        }
        
        .description {
            font-size: 1em;
            color: #74b9ff;
            margin-bottom: 20px;
        }
        
        .time-info {
            border-top: 1px solid #ddd;
            padding-top: 20px;
            margin-top: 20px;
        }
        
        .time {
            font-size: 1.2em;
            font-weight: 600;
            color: #2d3436;
        }
        
        .date {
            font-size: 0.9em;
            color: #636e72;
            margin-top: 5px;
        }
    </style>
</head>
<body>
    <div class="weather-card">
        <span class="weather-icon">{$weather_data['icon']}</span>
        <h1 class="temperature">{$weather_data['temp']}°</h1>
        <div class="condition">{$current_condition}</div>
        <div class="description">{$weather_data['desc']}</div>
        <div class="time-info">
            <div class="time">{$current_time}</div>
            <div class="date">{$current_date}</div>
        </div>
    </div>
</body>
</html>
HTML;

// Output HTTP headers in your format
$headers = "HTTP/1.1 200 OK\r\n";
$headers .= "Content-Type: text/html\r\n";
$headers .= "Connection: close\r\n";
$headers .= "Content-Length: " . strlen($body) . "\r\n";
$headers .= "\r\n";

echo $headers . $body;
?>