from sklearn.linear_model import LogisticRegression, LinearRegression
import pandas as pd

from bokeh.layouts import row
from bokeh.plotting import figure, output_file, show, curdoc
from bokeh.models import NumeralTickFormatter

curdoc().theme = "dark_minimal"

data = pd.read_csv("sensor_log.csv")

# Converting into percentage
data['Lamp Brightness'] = data['Lamp Brightness'] * 20 

X = data[["Timestamp","Lamp Brightness", "Movement", "Light (Lux)"]]  # Input features
y_brightness = (data['Lamp Brightness']) * 20 # Target: Brightness (0-100)

# Train models
brightness_model = LinearRegression().fit(X, y_brightness)

# Print model details
print("Brightness Weights:", brightness_model.coef_)
print("Brightness Intercept:", brightness_model.intercept_)


#____________________________ DATA VISUALIATION
time = data["Timestamp"]
brightness = data["Lamp Brightness"]
movement = data["Movement"]
lux = data["Light (Lux)"]

ambient_light_colors = [f"#{int((value * 255) / 100):02x}{150:02x}{150:02x}" for value in time]  
brightness_fill_alpha = [value / 100 for value in brightness]

#Lamp Brightness VS Time
p1 = figure()
p1.scatter(
    time, 
    brightness, 
    legend_label = "Lamp Brightness VS Time", 
    fill_color="grey",
    line_color = 'none',
    )

radii = brightness / 100
p1.circle(
    time,
    brightness,
    radius=radii,
    fill_color="white",
    fill_alpha=brightness_fill_alpha,
    line_color = 'none'
)

#Occupancy VS Time
p2 = figure()
p2.vbar( 
    x=time, 
    top=movement, 
    legend_label = "Occupancy VS Time", 
    width=0.1
    )

p2.yaxis.ticker = [0, 1]
p2.yaxis.major_label_overrides = {0: "Unoccupied", 1: "Occupied"}

#Ambient Light VS Time
p3 = figure()
p3.scatter(
    time, 
    lux, 
    legend_label = "Ambient Light VS Time", 
    fill_color = ambient_light_colors,
    line_color = 'none'
    )
p3.yaxis.formatter = NumeralTickFormatter(format="0,0")  # Integer format with commas

show(row(children=[p1,p2,p3]) , sizing_mode="scale_width")

