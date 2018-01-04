import tkinter as tk
import realtimemining as rtm


def sensor(sensor_id: str, label: tk.Label, image: tk.Label):
    """ Refreshes the label and image according to sensor data """

    value, error, alarm = rtm.read_sensor(sensor_id)

    label['text'] = '{}/100'.format(round(value, 2))

    if (sensor_id == 'AIR_FLOW' and value <= 20) or (sensor_id != 'AIR_FLOW' and value >= 80):
        label['fg'] = 'red'
        image['image'] = red
    else:
        label['fg'] = 'black'
        image['image'] = grey
    
    label.after(interval, sensor, sensor_id, label, image)


rtm.start_simulation()

root = tk.Tk()
root.title('Real-time Mining')

# Frame for the sensor data
sensor_frame = tk.Frame(root)
sensor_frame.grid(column=0, row=0, sticky=(tk.N, tk.W, tk.E, tk.S))
sensor_frame.columnconfigure(0, weight=1)
sensor_frame.rowconfigure(0, weight=1)

# Sensor frame header
alarm_label = tk.Label(sensor_frame, text='Alarm:')
value_label = tk.Label(sensor_frame, text='Value:')

alarm_label.grid(row=1, column=2, sticky=(tk.W, tk.E))
value_label.grid(row=1, column=3, sticky=(tk.W, tk.E))

# Sensor labels
ch4_label = tk.Label(sensor_frame, text='CH4:')
co_label = tk.Label(sensor_frame, text='CO:')
air_label = tk.Label(sensor_frame, text='Air:')

ch4_label.grid(row=2, column=1, sticky=tk.E)
co_label.grid(row=3, column=1, sticky=tk.E)
air_label.grid(row=4, column=1, sticky=tk.E)

# Sensor alarm images
grey = tk.PhotoImage(file='grey.png').subsample(4, 4)
red = tk.PhotoImage(file='red.png').subsample(4, 4)

# Sensor alarms
ch4_image = tk.Label(sensor_frame, image=grey)
co_image = tk.Label(sensor_frame, image=grey)
air_image = tk.Label(sensor_frame, image=grey)

ch4_image.grid(row=2, column=2, sticky=(tk.W, tk.E))
co_image.grid(row=3, column=2, sticky=(tk.W, tk.E))
air_image.grid(row=4, column=2, sticky=(tk.W, tk.E))

# Sensor values
ch4_value = tk.Label(sensor_frame, text='0/100')
co_value = tk.Label(sensor_frame, text='0/100')
air_value = tk.Label(sensor_frame, text='0/100')

ch4_value.grid(row=2, column=3, sticky=(tk.W, tk.E))
co_value.grid(row=3, column=3, sticky=(tk.W, tk.E))
air_value.grid(row=4, column=3, sticky=(tk.W, tk.E))

for child in sensor_frame.winfo_children():
    child.grid_configure(padx=5, pady=5)

# Sensor refresh interval
interval = 200

# Reads sensor data and refreshes labels and images
ch4_value.after(interval, sensor, 'CH4', ch4_value, ch4_image)
co_value.after(interval, sensor, 'CO', co_value, co_image)
air_value.after(interval, sensor, 'AIR_FLOW', air_value, air_image)

root.mainloop()

