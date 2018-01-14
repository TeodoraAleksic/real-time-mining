import tkinter as tk
import realtimemining as rtm


# Sensor refresh interval
INTERVAL = 200


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
water_tank_value_label = tk.Label(water_frame, text='0/100')

water_tank_label.grid(row=0, column=0, sticky=(tk.W, tk.E))
water_tank_value_label.grid(row=0, column=1, sticky=(tk.W, tk.E))

# Water level alarms
high_level_label = tk.Label(water_frame, text='High level:')
low_level_label = tk.Label(water_frame, text='Low level:')

high_level_label.grid(row=1, column=0, sticky=(tk.W, tk.E))
low_level_label.grid(row=1, column=1, sticky=(tk.W, tk.E))

high_level_image = tk.Label(water_frame, image=grey)
low_level_image = tk.Label(water_frame, image=grey)

high_level_image.grid(row=2, column=0, sticky=(tk.W, tk.E))
low_level_image.grid(row=2, column=1, sticky=(tk.W, tk.E))

# Pump controls
pump_label = tk.Label(water_frame, text='Pump on/off:')
pump_label.grid(row=3, column=0, sticky=(tk.W, tk.E))

pump_image = tk.Label(water_frame, image=grey)
pump_image.grid(row=3, column=1, sticky=(tk.W, tk.E))

# Water tank frame padding
for child in water_frame.winfo_children():
    child.grid_configure(padx=5, pady=5)

# Reads water tank and pump control data
water_tank_value_label.after(INTERVAL, water_tank, water_tank_value_label)
high_level_image.after(INTERVAL, water_level, high_level_image, low_level_image)
pump_image.after(INTERVAL, pump_control, pump_image)

# Frame for the sensor data
sensor_frame = tk.Frame(root)
sensor_frame.grid(row=0, column=1, sticky=(tk.N, tk.W, tk.E, tk.S))
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
ch4_value = tk.Label(sensor_frame, text='0/100')
co_value = tk.Label(sensor_frame, text='0/100')
air_value = tk.Label(sensor_frame, text='0/100')

ch4_value.grid(row=2, column=3, sticky=(tk.W, tk.E))
co_value.grid(row=3, column=3, sticky=(tk.W, tk.E))
air_value.grid(row=4, column=3, sticky=(tk.W, tk.E))

# Sensor frame padding
for child in sensor_frame.winfo_children():
    child.grid_configure(padx=5, pady=5)

# Reads sensor data and refreshes labels and images
ch4_value.after(INTERVAL, sensor, 'CH4', ch4_value, ch4_image)
co_value.after(INTERVAL, sensor, 'CO', co_value, co_image)
air_value.after(INTERVAL, sensor, 'AIR_FLOW', air_value, air_image)

root.mainloop()

