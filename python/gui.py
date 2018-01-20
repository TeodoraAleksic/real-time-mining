import tkinter as tk
import realtimemining as rtm


# Sensor refresh interval
INTERVAL = 150


def water_tank(label: tk.Label):
    """ Refreshes the water tank level label """

    value = rtm.get_water_level()

    label['text'] = '{}/100'.format(round(value, 2))

    if value <= 20 or value >= 80:
        label['fg'] = 'red'
    else:
        label['fg'] = 'black'

    label.after(INTERVAL, water_tank, label)


def water_level(high_level: tk.Label, low_level: tk.Label):
    """ Refreshes the water level alarms """

    high_level_alarm, low_level_alarm = rtm.get_water_level_alarms()

    if high_level_alarm:
        high_level['image'] = red
    else:
        high_level['image'] = grey

    if low_level_alarm:
        low_level['image'] = red
    else:
        low_level['image'] = grey

    high_level.after(INTERVAL, water_level, high_level, low_level)


def pump_control(image: tk.Label):
    """ Refreshes the pump on/off sign """

    pump_on = rtm.is_pump_on()

    if pump_on:
        image['image'] = red
    else:
        image['image'] = grey

    image.after(INTERVAL, pump_control, image)


def sensor(sensor_id: str, label: tk.Label, image: tk.Label):
    """ Refreshes the label and image according to sensor data """

    value, alarm = rtm.get_sensor_data(sensor_id)

    label['text'] = '{}/100'.format(round(value, 2))

    if (sensor_id == 'AIR_FLOW' and value <= 20) or (sensor_id != 'AIR_FLOW' and value >= 80):
        label['fg'] = 'red'
        image['image'] = red
    else:
        label['fg'] = 'black'
        image['image'] = grey

    if alarm:
        image['image'] = red
    else:
        image['image'] = grey
    
    label.after(INTERVAL, sensor, sensor_id, label, image)


def console(text: tk.Text):
    """ Refreshes the logging console """

    messages = rtm.get_logging_data()

    text.insert(tk.END, messages)
    text.see(tk.END)

    text.after(INTERVAL, console, text)


rtm.start_simulation()

root = tk.Tk()
root.title('Real-time Mining')

# Alarm images
grey = tk.PhotoImage(file='grey.png').subsample(4, 4)
red = tk.PhotoImage(file='red.png').subsample(4, 4)

# Frame for water tank data
water_frame = tk.Frame(root)
water_frame.grid(row=0, column=0, sticky=(tk.N, tk.W, tk.E, tk.S))
water_frame.columnconfigure(0, weight=1)
water_frame.rowconfigure(0, weight=1)

# Water tank
water_tank_label = tk.Label(water_frame, text='Water tank:')
water_tank_value_label = tk.Label(water_frame, text='0/100', width=10)

water_tank_label.grid(row=0, column=0, sticky=tk.E)
water_tank_value_label.grid(row=0, column=1, sticky=tk.E)

# Pump controls
pump_image = tk.Label(water_frame, image=grey)
pump_label = tk.Label(water_frame, text='Pump on/off:')

pump_label.grid(row=1, column=0, sticky=tk.E)
pump_image.grid(row=1, column=1, sticky=(tk.W, tk.E))

# Water level alarms
high_level_label = tk.Label(water_frame, text='High level:')
high_level_image = tk.Label(water_frame, image=grey)

high_level_label.grid(row=2, column=0, sticky=tk.E)
high_level_image.grid(row=2, column=1, sticky=(tk.W, tk.E))

low_level_label = tk.Label(water_frame, text='Low level:')
low_level_image = tk.Label(water_frame, image=grey)

low_level_label.grid(row=3, column=0, sticky=tk.E)
low_level_image.grid(row=3, column=1, sticky=(tk.W, tk.E))

# Turn pump on/off buttons
turn_on = tk.Button(water_frame, text="Turn on", command=rtm.turn_pump_on, width=10)
turn_off = tk.Button(water_frame, text="Turn off", command=rtm.turn_pump_off, width=10)

turn_on.grid(row=4, column=0, sticky=(tk.W, tk.E))
turn_off.grid(row=4, column=1, sticky=(tk.W, tk.E))

# Reads water tank and pump control data
water_tank_value_label.after(INTERVAL, water_tank, water_tank_value_label)
high_level_image.after(INTERVAL, water_level, high_level_image, low_level_image)
pump_image.after(INTERVAL, pump_control, pump_image)

# Water tank frame padding
for child in water_frame.winfo_children():
    child.grid_configure(padx=5, pady=5)

# Frame for the sensor data
sensor_frame = tk.Frame(root)
sensor_frame.grid(row=0, column=1, sticky=(tk.N, tk.W, tk.E))
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

# Sensor alarms
ch4_image = tk.Label(sensor_frame, image=grey)
co_image = tk.Label(sensor_frame, image=grey)
air_image = tk.Label(sensor_frame, image=grey)

ch4_image.grid(row=2, column=2, sticky=(tk.W, tk.E))
co_image.grid(row=3, column=2, sticky=(tk.W, tk.E))
air_image.grid(row=4, column=2, sticky=(tk.W, tk.E))

# Sensor values
ch4_value = tk.Label(sensor_frame, text='0/100', width=10)
co_value = tk.Label(sensor_frame, text='0/100', width=10)
air_value = tk.Label(sensor_frame, text='0/100', width=10)

ch4_value.grid(row=2, column=3, sticky=tk.E)
co_value.grid(row=3, column=3, sticky=tk.E)
air_value.grid(row=4, column=3, sticky=tk.E)

# Reads sensor data and refreshes labels and images
ch4_value.after(INTERVAL, sensor, 'CH4', ch4_value, ch4_image)
co_value.after(INTERVAL, sensor, 'CO', co_value, co_image)
air_value.after(INTERVAL, sensor, 'AIR_FLOW', air_value, air_image)

# Sensor frame padding
for child in sensor_frame.winfo_children():
    child.grid_configure(padx=5, pady=5)

# Frame for the console data
console_frame = tk.Frame(root)
console_frame.grid(row=1, column=0, columnspan=2, sticky=(tk.N, tk.W, tk.E))
console_frame.columnconfigure(0, weight=1)
console_frame.rowconfigure(0, weight=1)

# Text area for console data
text_area = tk.Text(console_frame, height=8, width=45)
text_area.after(INTERVAL, console, text_area)

# Console frame padding
for child in console_frame.winfo_children():
    child.grid_configure(padx=5, pady=5)

root.mainloop()

