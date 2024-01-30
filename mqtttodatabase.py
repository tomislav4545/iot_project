import MySQLdb as mdb
import time
import paho.mqtt.client as mqtt

mqtt_broker = "localhost"
mqtt_port = 1883
mqtt_topics = ["sensors/motion", "sensors/code", "sensors/gas"]

db_host = "localhost"
db_user = "sensor_writer"
db_password = "password"
db_name = "sensors"

def on_message(client, userdata, msg):
    payload = msg.payload.decode('utf-8')
    print(f"Received message on topic {msg.topic}: {payload}")

    con = mdb.connect(db_host, db_user, db_password, db_name)
    cursor = con.cursor()
    sql = "INSERT INTO sensorData(datum, vrijeme, message) VALUES ('%s', '%s', '%s')" % (
        time.strftime("%Y-%m-%d"),
        time.strftime("%H:%M:%S"),
        payload,
    )
    cursor.execute(sql)
    con.commit()
    con.close()

client = mqtt.Client()
client.on_message = on_message

client.connect(mqtt_broker, mqtt_port, 60)
for topic in mqtt_topics:
    client.subscribe(topic)

client.loop_forever()
