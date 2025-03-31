document.addEventListener('DOMContentLoaded', function() {
    const logElement = document.getElementById('requestLog');
    const sendButton = document.getElementById('sendButton');
    const testButton = document.getElementById('testButton');
    const clearLogButton = document.getElementById('clearLogButton');
    const jsonInput = document.getElementById('jsonInput');
    let socket;
    let reconnectAttempts = 0;
    const maxReconnectAttempts = 5;

    function addLogEntry(message, data = null) {
        const entry = document.createElement('div');
        entry.className = 'log-entry';
        
        const timeElement = document.createElement('div');
        timeElement.className = 'log-time';
        timeElement.textContent = new Date().toLocaleTimeString();
        
        const dataElement = document.createElement('div');
        dataElement.className = 'log-data';
        
        if (data) {
            try {
                const jsonData = JSON.parse(data);
                dataElement.textContent = JSON.stringify(jsonData, null, 2);
            } catch (e) {
                dataElement.textContent = data;
            }
        } else {
            dataElement.textContent = message;
        }
        
        entry.appendChild(timeElement);
        entry.appendChild(dataElement);
        logElement.prepend(entry);
        
        if (logElement.children.length > 20) {
            logElement.removeChild(logElement.lastChild);
        }
    }

    function connectWebSocket() {
        const protocol = window.location.protocol === 'https:' ? 'wss://' : 'ws://';
        const wsUri = protocol + window.location.hostname + ':81';
        
        socket = new WebSocket(wsUri);

        socket.onopen = function() {
            addLogEntry("WebSocket connection established");
            reconnectAttempts = 0;
        };

        socket.onmessage = function(event) {
            addLogEntry("Received data:", event.data);
        };

        socket.onclose = function(event) {
            if (event.wasClean) {
                addLogEntry(`Connection closed cleanly, code=${event.code}, reason=${event.reason}`);
            } else {
                addLogEntry('Connection interrupted');
            }
            
            if (reconnectAttempts < maxReconnectAttempts) {
                reconnectAttempts++;
                const delay = Math.min(5000, reconnectAttempts * 1000);
                addLogEntry(`Attempting to reconnect (${reconnectAttempts}/${maxReconnectAttempts})...`);
                setTimeout(connectWebSocket, delay);
            } else {
                addLogEntry('Max reconnection attempts reached');
            }
        };

        socket.onerror = function(error) {
            addLogEntry('WebSocket error: ' + error.message);
        };
    }

    function sendJsonData(jsonString) {
        try {
            JSON.parse(jsonString);
            
            fetch('/api/log', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: jsonString
            })
            .then(response => response.json())
            .then(data => {
                addLogEntry("Request successful:", JSON.stringify(data));
            })
            .catch(error => {
                addLogEntry("Request failed: " + error.message);
            });
            
        } catch (e) {
            addLogEntry("Invalid JSON: " + e.message);
        }
    }

    // Инициализация
    connectWebSocket();
    addLogEntry("System initialized");
    
    // Заполняем поле примером JSON
    jsonInput.value = JSON.stringify({
        "command": "test",
        "parameters": {
            "value": 123,
            "mode": "auto"
        },
        "timestamp": new Date().toISOString()
    }, null, 2);

    // Обработчики событий
    clearLogButton.addEventListener('click', function() {
        logElement.innerHTML = '<div class="log-entry">Log cleared</div>';
        addLogEntry("Log cleared by user");
    });

    sendButton.addEventListener('click', function() {
        const jsonData = jsonInput.value.trim();
        if (jsonData) {
            addLogEntry("Sending custom JSON...");
            sendJsonData(jsonData);
        } else {
            addLogEntry("Please enter JSON data");
        }
    });

    testButton.addEventListener('click', function() {
        const testData = {
            action: "test",
            value: Math.random().toFixed(4),
            timestamp: new Date().toISOString(),
            source: "browser"
        };
        jsonInput.value = JSON.stringify(testData, null, 2);
        addLogEntry("Sending test JSON...");
        sendJsonData(JSON.stringify(testData));
    });
});