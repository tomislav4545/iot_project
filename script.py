from flask import Flask, render_template
from flask_socketio import SocketIO
import json
import paho.mqtt.client as mqtt

app = Flask(__name__)
socketio = SocketIO(app)

mqtt_broker_address = "localhost"
mqtt_broker_port = 1883
mqtt_topics = ["sensors/gas", "sensors/motion", "sensors/code"]

mqtt_client = mqtt.Client()


def on_connect(client, userdata, flags, rc):
    print(f"Connected to MQTT broker with result code {rc}")
    for topic in mqtt_topics:
        client.subscribe(topic)


def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode("utf-8")
    print(f"Received message on topic {topic}: {payload}")

    socketio.emit("mqtt_message", {"topic": topic, "message": payload})


mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

mqtt_client.connect(mqtt_broker_address, mqtt_broker_port, 60)

mqtt_client.loop_start()
socketio.emit("mqtt_message","poruka")

@app.route('/')
def index():
    return render_template('index.html')


@socketio.on('connect')
def handle_connect():
    print('Client connected')


if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000, debug=True)

