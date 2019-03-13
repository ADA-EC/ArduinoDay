import paho.mqtt.client as mqtt
import time

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
	print("Connected with result code "+str(rc))

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
	print(msg.topic+" "+str(msg.payload))

# Callback for subscribing
def on_subscribe(client, userdata, mid, granted_qos):
	#print("Success subscribing.")
	pass

# Callback from publishing
def on_publish(client, userdata, mid):
	#print("Success publishing.")
	pass

client = mqtt.Client()
client.username_pw_set("zwrbsgco", password="Gu19Jq6j-ruy")
client.reconnect_delay_set(max_delay=60)
client.on_connect = on_connect
client.on_message = on_message
client.on_publish = on_publish
client.on_subscribe = on_subscribe

try:
	client.connect("m11.cloudmqtt.com", 18002, keepalive=300)
except ConnectionError:
	print("Could not connect to the Broker. Please check if you computer is connected and try again.")
	quit()
except Exception:
	print("An error has occurred. Try again.")
	quit()

client.loop_start()
	

while(True):
	msg = input()
	client.publish("ADA/ArduinoDay", payload=msg, qos=1)