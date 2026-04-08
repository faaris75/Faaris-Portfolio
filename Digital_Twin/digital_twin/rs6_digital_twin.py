import streamlit as st
import json
import subprocess
import pandas as pd
import os
import platform

st.set_page_config(page_title="RS 6 Digital Twin Simulator", layout="wide")

st.markdown("""
<style>
.stSlider, .stNumberInput, .stCheckbox { background-color: #1e1e1e !important; border-radius: 8px; padding: 10px; color: #FFFFFF; }
.green-score { color: #00FF00 !important; font-weight: bold; }
h1 { font-family: 'Arial', sans-serif; text-transform: uppercase; color: #FFFFFF; }
</style>
""", unsafe_allow_html=True)

def run_cpp_digital_twin(passengers, cargo, fuel, speed, windows, roof, rpm, engine_load, ambient, distance, slope, price):
    # Convert booleans (True/False) to 1 or 0 for C++ 
    win_int = 1 if windows else 0
    roof_int = 1 if roof else 0
    
    # Format the exact 12 inputs matching the C++ std::cin order
    input_string = f"{passengers}\n{cargo}\n{fuel}\n{speed}\n{win_int}\n{roof_int}\n{rpm}\n{engine_load}\n{ambient}\n{distance}\n{slope}\n{price}\n"
    
    # Handle Windows vs Mac/Linux executable naming
    exe_name = 'a.exe' if platform.system() == 'Windows' else './a.exe'
    
    try:
        process = subprocess.run([exe_name], input=input_string, capture_output=True, text=True)
        output = process.stdout
        json_start = output.find('{')
        
        if json_start != -1:
            return json.loads(output[json_start:])
        else:
            st.error("C++ output error. Check terminal.")
            st.text("Raw C++ Output:\n" + output)
            if process.stderr:
                st.text("C++ Error Output:\n" + process.stderr)
            return None
    except FileNotFoundError:
        st.error(f"Could not find {exe_name}. Did you run the g++ compile command first?")
        return None
    except Exception as e:
        st.error(f"Execution failed: {e}")
        return None

# --- UI LAYOUT ---
st.title("RS 6 DIGITAL TWIN: FUEL EFFICIENCY SIMULATOR")

col_inputs, col_main = st.columns([1, 3])

with col_inputs:
    st.header("INPUTS")
    
    with st.expander("1. WEIGHT CONTRIBUTION", expanded=True):
        p_kg = st.number_input("PASSENGERS (kg)", 0.0, 1000.0, 250.0)
        c_kg = st.number_input("CARGO (kg)", 0.0, 1000.0, 100.0)
        fuel_fraction = st.slider("FUEL LEVEL (fraction)", 0.0, 1.0, 0.8)

    with st.expander("2. AERODYNAMIC DRAG", expanded=True):
        speed_in = st.slider("Vehicle Speed (km/h)", 0.0, 300.0, 110.0)
        win_open = st.checkbox("Windows Open?")
        roof_load = st.checkbox("Roof Rack/Box Attached?")

    with st.expander("3. ENGINE THERMALS", expanded=True):
        rpm_in = st.slider("Engine RPM", 800, 7000, 2500)
        load_in = st.slider("Engine Load / Throttle", 0.0, 1.0, 0.4)
        ambient_in = st.number_input("Ambient Temp (C)", -30.0, 50.0, 20.0)

    with st.expander("4. ROUTE & MILEAGE", expanded=True):
        dist_in = st.number_input("Trip Distance (km)", 1.0, 1000.0, 50.0)
        slope_in = st.slider("Average Incline (Degrees)", -15.0, 15.0, 0.0)
        price_in = st.number_input("Fuel Price ($/L)", 0.50, 3.00, 1.65)

with col_main:
    st.header("DIGITAL TWIN INTERFACE")

    # Send ALL 12 variables to C++
    sim_data = run_cpp_digital_twin(p_kg, c_kg, fuel_fraction, speed_in, win_open, roof_load, rpm_in, load_in, ambient_in, dist_in, slope_in, price_in)

    if sim_data:
        scores = sim_data['scores']
        wb = sim_data['weight_breakdown']

        st.subheader("FUEL EFFICIENCY SCORE")
        st.markdown(f"""
        <div style='text-align: center;'>
            <p style='font-size: 80px; font-weight: bold; color: #00FF00;'>
                {scores['overall_points']:.1f} / 100
            </p>
        </div>
        """, unsafe_allow_html=True)
        
        # Ensure progress bar stays within 0-100 bounds
        progress_val = int(max(0, min(100, scores['overall_points'])))
        st.progress(progress_val)

        st.subheader("MODULE PERFORMANCE (pts)")
        c1, c2, c3, c4, c5 = st.columns(5)
        c1.metric("Weight", f"{scores['weight_points']:.1f}")
        c2.metric("Aero (Drag)", f"{scores['aerodynamics_points']:.1f}")
        c3.metric("Engine Temp", f"{scores['engine_points']:.1f}")
        c4.metric("Route/Speed", f"{scores['speed_points']:.1f}")
        c5.metric("Tires", f"{scores['tire_points']:.1f}")

        st.markdown("---")
        st.subheader("TELEMETRY (From C++)")
        m1, m2, m3, m4, m5 = st.columns(5)
        m1.metric("Drag Force (N)", f"{wb['drag_force_N']:.0f}")
        m2.metric("Coolant Temp (C)", f"{wb['coolant_temp_c']:.1f}")
        m3.metric("Added Mass (kg)", f"{wb['added_mass_kg']:.0f}")
        m4.metric("Fuel Rate (L/100km)", f"{wb['fuel_consumption_L_per_100km']:.1f}")
        m5.metric("Trip Est. Cost ($)", f"{wb['trip_cost']:.2f}")

st.markdown("---")
st.subheader("SIMULATION HISTORY & TRENDS")

if os.path.exists("simulation_history.csv"):
    try:
        column_names = ["Speed (km/h)", "RPM", "Overall Score", "Fuel (L/100km)"]
        df = pd.read_csv("simulation_history.csv", names=column_names)
        
        col_graph, col_table = st.columns([2, 1])
        
        with col_graph:
            st.markdown("**Overall Score Trend**")
            st.line_chart(df["Overall Score"])
            
        with col_table:
            st.markdown("**Raw Data**")
            st.dataframe(df, use_container_width=True, hide_index=True)
            
    except Exception as e:
        st.warning(f"Could not read the CSV file yet. Error: {e}")
else:
    st.info("No simulation history found. Change a slider to generate the first data point!")