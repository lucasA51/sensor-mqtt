import paho.mqtt.client as mqtt
import sqlite3
from datetime import datetime
from zoneinfo import ZoneInfo
import os

# Ensure the 'data' directory exists in the current working directory
os.makedirs('data', exist_ok=True)

# Banco SQLite salvo no volume montado em data
conn = sqlite3.connect(os.path.join('data', 'leituras.db'), check_same_thread=False)
cursor = conn.cursor()

cursor.execute("""
CREATE TABLE IF NOT EXISTS leituras (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    distancia REAL,
    timestamp TEXT
)
""")
conn.commit()

def on_connect(client, userdata, flags, rc):
    print("Conectado ao MQTT.")
    client.subscribe("sensor/distancia")

def on_message(client, userdata, msg):
    try:
        distancia = float(msg.payload.decode())
        timestamp = datetime.now(ZoneInfo("America/Sao_Paulo")).isoformat()
        cursor.execute("INSERT INTO leituras (distancia, timestamp) VALUES (?, ?)", (distancia, timestamp))
        conn.commit()
        print(f"Salvo: {distancia} cm em {timestamp}")
    except Exception as e:
        print("Erro ao salvar:", e)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("test.mosquitto.org", 1883, 60)
client.loop_forever()