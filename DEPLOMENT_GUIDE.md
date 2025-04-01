# ############################ #
# A. Загрузка через USB        #
# ############################ #

# Прошить файловую систему (LittleFS)  
pio run --target uploadfs  

# Прошить прошивку  
pio run --target upload  

# Комбинированная команда (рекомендуется)  
pio run --target uploadfs && pio run --target upload  

# ############################ #
# B. Загрузка по воздуху (OTA) #
# ############################ #

# Проверить доступность устройства  
ping 192.168.0.26 

# Прошить файловую систему  
pio run --target uploadfs --upload-port 192.168.0.26  

# Прошить прошивку  
pio run --target upload --upload-port 192.168.0.26  

# Комбинированная команда  
pio run --target uploadfs --upload-port 192.168.1.100 && \  
pio run --target upload --upload-port 192.168.1.100  

# ############################ #
# C. Полезные команды          #
# ############################ #

# Открыть Serial Monitor (115200 baud)  
pio device monitor  

# Очистить проект  
pio run --target clean  

# Список COM-портов (Windows)  
pio device list  