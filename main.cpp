#include <iostream>
#include <iomanip>

struct _capacitor
{
   double *time;       // time array
   double *voltage;    // voltage array
   double *current;    // current array
   double C;           // capacitance value
};
typedef struct _capacitor Capacitor;

bool allocateCapacitor(Capacitor &cap, double c_val, int num_points)     // use bool to find if there is (NULL or 0) to false, true when its a whole or real number
{
    cap.C = c_val; // Save capacitance into the struct

    // Allocate Memory
    cap.time = new (std::nothrow) double[num_points];
    cap.voltage = new (std::nothrow) double[num_points];
    cap.current = new (std::nothrow) double[num_points];

    
    if(!cap.time || !cap.voltage || !cap.current)   // allocation failed, clean up the others and return false
    {
	delete[] cap.time;
	delete[] cap.voltage;
	delete[] cap.current;
        return false;
    }
    return true;  // Pass the if function, safely continues
}



// Free memory to prevent memory leaks
void freeCapacitor(Capacitor &cap)
{
    delete[] cap.time;
    delete[] cap.voltage;
    delete[] cap.current;
    cap.time = nullptr;
    cap.voltage = nullptr;
    cap.current = nullptr;
}



int main()
{
   
   const double dt = 1e-10;  // Time step size
   const double t_final = 5e-6;  // total simulation time (5e-6 = 5 x 10^-6)
   const int num_steps = static_cast<int>(t_final/dt);  // total steps
   const int num_points = num_steps + 1;  // total  array size (0 to 50000)

   
   // Set values for R, C_val, I_const, V0
   const double R = 1000.0;
   const double C_val = 100e-12;
   const double I_const = 1e-2;
   const double V0 = 10.0;


   Capacitor cap_cc;   // Constant current circuit
   Capacitor cap_cv;   // Constant voltage circuit

   if(!allocateCapacitor(cap_cc, C_val, num_points) || !allocateCapacitor(cap_cv, C_val, num_points))    // Allocate memory for both circuits
   {
       std::cerr << "Memory allocation failed.\n";
       freeCapacitor(cap_cc);
       freeCapacitor(cap_cv);
       return 1;
   }

   // start values at t = 0
   cap_cc.time[0] = 0.0;
   cap_cv.time[0] = 0.0;

   // constant current start values
   cap_cc.current[0] = I_const;
   cap_cv.current[0] = V0 / R;

   // constant voltage start values
   cap_cc.voltage[0] = 0.0;
   cap_cv.voltage[0] = V0 - (cap_cv.current[0] * R);


   for (int t = 0; t<num_steps; ++t)   // Loop through time to calculate the capacitor's voltage cand current as they change over 50000 steps
   {

       // update time
       cap_cc.time[t+1] = (t+1) * dt;
       cap_cv.time[t+1] = (t+1) * dt;
   

       // calculate constant current circuit
       cap_cc.current[t+1] = I_const;
       cap_cv.current[t+1] = cap_cv.current[t] - (cap_cv.current[t] / (R * cap_cv.C)) * dt;


       // calculate constant voltage circuit
       cap_cc.voltage[t+1] = cap_cc.voltage[t] + cap_cc.current[t] * dt * (1.0 / cap_cc.C);
       cap_cv.voltage[t+1] = cap_cv.voltage[t] + cap_cv.current[t] * dt * (1.0 / cap_cv.C);
   }

       std::cout << std::scientific << std::setprecision(5);  // Set output format


       // Print out title and setup the table header and column names
       // spacing out neatly use "setw"       
       std::cout << "\n--- Constant Current Source ---\n";
       std::cout << std::setw(8) << "Step"
	         << std::setw(16) << "Time (s)"
		 << std::setw(16) << "Voltage (V)"
		 << std::setw(16) << "Current (A)\n";

       for(int t = 0; t <= num_steps; t += 200)   // loop print constant current circuit every 200 steps
       {
           std::cout << std::setw(8) << t
		     << std::setw(16) << cap_cc.time[t]
		     << std::setw(16) << cap_cc.voltage[t]
		     << std::setw(16) << cap_cc.current[t] << "\n";
       }

       // print out title and setyp the table header and column names
       std::cout << "\n--- Constant Voltage Source ---\n";
       std::cout << std::setw(8) << "Step"
                 << std::setw(16) << "Time (s)"
                 << std::setw(16) << "Voltage (V)"
                 << std::setw(16) << "Current (A)\n";

       for(int t = 0; t <= num_steps; t += 200)  // loop print constant voltage circuit every 200 steps
       {
           std::cout << std::setw(8) << t
                     << std::setw(16) << cap_cv.time[t]
                     << std::setw(16) << cap_cv.voltage[t]
                     << std::setw(16) << cap_cv.current[t] << "\n";
       }
       
       // free memory at the end
       delete[] cap_cc.time;
       delete[] cap_cc.voltage;;
       delete[] cap_cc.current;
       delete[] cap_cv.time;
       delete[] cap_cv.voltage;
       delete[] cap_cv.current;

       return 0;
}
