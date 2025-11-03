#include <iostream>
#include <cmath>
#include <iomanip>

// valuating an up-and-out American barrier put option where S = 50, X= 51, barrier = 56, rebate = 0, 
// r = 0.06, q = 0.01, sigma = 0.2, t = 1 and N = 4

double price; // asset price
double strike; // strike price
double barrier; // barrier price
double rate; // risk-free interest rate
double dividend; // dividend yield
double vol; // volatility
double rebate; // rebate if barrier is hit
double T; // time to maturity
int N; // number of time steps
char exercise; // 'A'merican or 'E'uropen

double BarrierOption(double price, double strike, double barrier, double rate, double dividend, double vol, double rebate, double T, int N, char exercise)
{
    int i, j ; //counters
    double pd; // down probability
    double pm; // middle probability
    double pu; // up probability
    double s[120] [100]; // stock prices
    double p[120] [100]; // put prices
    double up  = 0.0; // up movement
    double down = 0.0; // down movement 
    double dt = T / N; // length of time step   
    double drift = rate - dividend - vol*vol/2; //drift rate
    double dx = vol * sqrt(3*dt); // state step

    //computing risk neutral probabilities
    pu = sqrt(dt/12) * (drift/vol) + 0.16667;
    pd = -sqrt(dt/12) * (drift/vol) + 0.16667;
    pm = 1 - pu -pd;
    down = 1/up;

    //computing stock price at each node
    for (i =N; i >=0; i--){
        for(j = -i; j <= i; j++){
            s[i][j] = price * pow(up, j);
            
        }
    }

    //payoff at maturity

    for (j = N; j >= -N; j--)
{
if (s[N][j] < barrier)
p[N][j] = strike - s[N][j];
else
p[N][j] = rebate;
}

// compute payoffs at all other time steps
for(i=N; i >= 0; i--){
    for(j=i; j>= -i; j--){
        if (s[i][j] < barrier){
            p[i][j] = exp(-rate*dt)*(pu*p[i+1][j+1] + pm*p[i+1][j] + pd*p[i+1][j-1]);
            if(exercise == 'A'){
                p[i][j] = std::max(p[i][j], strike - s[i][j]);
                }
                else{
                    p[i][j] = rebate;

                // Derman Kani Adjustment
                if  ((s[i][j] < barrier) && (s[i][j+1] >= barrier)){
                    p[i][j] = (rebate-p[i][j])/(s[i][j+1] - s[i][j])*(barrier - s[i][j]);

                }


                }
            }
         }
    }
    return p[0][0];
}


int main()
{
    // Parameters
    double S = 50.0;       
    double K = 51.0;       
    double B = 56.0;       
    double r = 0.06;       
    double q = 0.01;      
    double sigma = 0.2;    
    double rebate = 0.0;  
    double T = 1.0;        
    int N = 4;             
    char exercise = 'A';   

    double price = BarrierOption(S, K, B, r, q, sigma, rebate, T, N, exercise);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Up-and-Out American Put Option Price: " << price << std::endl;

    return 0;
}