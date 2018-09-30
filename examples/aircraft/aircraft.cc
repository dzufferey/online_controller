/*
 * aircraft.cc
 *
 *  created: Oct 2016
 *  author: Matthias Rungger
 *
 */

/*
 * information about this example is given in
 * http://arxiv.org/abs/1503.03715
 * doi: 10.1109/TAC.2016.2593947
 * doi: 10.1109/CDC.2015.7403185
 *
 */

#include <iostream>
#include <array>

/* SCOTS header */
#include "scots.hh"
/* ode solver */
#include "RungeKutta4.hh"

/* time profiling */
#include "TicToc.hh"
/* memory profiling */
#include <sys/time.h>
#include <sys/resource.h>
struct rusage usage;

/* state space dim */
const int state_dim=3;
/* input space dim */
const int input_dim=2;
/* sampling time */
const double tau = 0.25;
using abs_type = scots::abs_type;
/* data types of the state space elements and input 
 * space elements used in uniform grid and ode solver */
using state_type = std::array<double,state_dim>;
using input_type = std::array<double,input_dim>;
using disturbance_type = std::array<double, state_dim>;
using ds_type = std::array<double, 2*state_dim>;


int main() {
  /* to measure time */
  TicToc tt;

  /* construct grid for the state space */
  /* setup the workspace of the synthesis problem and the uniform grid */
  /* grid node distance diameter */
  /* optimized values computed according to doi: 10.1109/CDC.2015.7403185 */
  state_type s_eta={{25.0/362,3*M_PI/180/66,56.0/334}}; 
  /* lower bounds of the hyper rectangle */
  state_type s_lb={{58,-3*M_PI/180,0}};
  /* upper bounds of the hyper rectangle */
  state_type s_ub={{70,0,55}}; 
  scots::UniformGrid ss(state_dim,s_lb,s_ub,s_eta);
  std::cout << "Uniform grid details:" << std::endl;
  ss.print_info();

  /* construct grid for the input space */
  /* lower bounds of the hyper rectangle */
  input_type i_lb={{0,0}};
  /* upper bounds of the hyper rectangle */
  input_type i_ub={{32000,8*M_PI/180}};
  /* grid node distance diameter */
  input_type i_eta={{32000,9.0/8.0*M_PI/180}};
  scots::UniformGrid is(input_dim,i_lb,i_ub,i_eta);
  is.print_info();

  /* setup object to compute the transition function */
  scots::Abstraction<state_type,input_type,ds_type> abs(ss,is);
  /* measurement disturbances  */
  state_type z={{0.0125,0.0025/180*M_PI,0.05}};
  abs.set_measurement_error_bound(z);

  disturbance_type w_1={.108,0.002,0};
  disturbance_type w_2={0.203, 0.001, 0.001};
  disturbance_type w2_lb={58,-3*M_PI/180,0};
  disturbance_type w2_ub={70,0,4};

  scots::Disturbance<disturbance_type, state_type> dis(w_1, ss);

  auto rs_post = [&dis](ds_type &y, input_type &u) -> void {
  auto rhs =[&dis](ds_type &yy, const ds_type &y, input_type &u) -> void {
    /* find the distrubance for the given state */
    state_type x;
    state_type r;
    for (int i=0; i<state_dim; i++){
      x[i] = y[i];
      r[i] = y[i+state_dim];
    }
    disturbance_type w = dis.get_disturbance(x,r);

    double mg = 60000.0*9.81;
    double mi = 1.0/60000;
    double c=(1.25+4.2*u[1]);
    double L[3][2];
    L[0][0]=-0.00191867*(2.7+3.08*(1.25+4.2*u[1])*(1.25+4.2*u[1]));
    L[0][1]=9.81;
    L[1][0]=0.002933+0.004802*u[1];
    L[1][1]=0.003623;
    L[2][0]=0.07483;
    L[2][1]=83.22;
    yy[0] = mi*(u[0]*std::cos(u[1])-(2.7+3.08*c*c)*y[0]*y[0]-mg*std::sin(y[1]));
    yy[1] = (1.0/(60000*y[0]))*(u[0]*std::sin(u[1])+68.6*c*y[0]*y[0]-mg*std::cos(y[1]));
    yy[2] = y[0]*std::sin(y[1]);
    /* to account for input disturbances */
    yy[3] = L[0][0]*y[3]+L[0][1]*y[4]+w[0]; /* L[0][2]=0 */
    yy[4] = L[1][0]*y[3]+L[1][1]*y[4]+w[1]; /* L[1][2]=0 */
    yy[5] = L[2][0]*y[3]+L[2][1]*y[4]+w[2]; /* L[2][2]=0 */
  };
  scots::runge_kutta_fixed4(rhs,y,u,2*state_dim,tau,10);
};

auto rs_repost = [&dis,w2_lb,w2_ub](ds_type &y, input_type &u, bool &neigbour) -> void {
  dis.set_intersection_check();
  //dis.set_out_of_domain();
  auto rhs =[&dis,w2_lb,w2_ub](ds_type &yy, const ds_type &y, input_type &u) -> void {
    /* find the distrubance for the given state */
    state_type x;
    state_type r;
    for (int i=0; i<state_dim; i++){
      x[i] = y[i];
      r[i] = y[i+state_dim];
    }
    disturbance_type w = dis.get_disturbance(x,r);
   
    dis.intersection(x,r, w2_lb,w2_ub);
    
    double mg = 60000.0*9.81;
    double mi = 1.0/60000;
    double c=(1.25+4.2*u[1]);
     double L[3][2];
    L[0][0]=-0.00191867*(2.7+3.08*(1.25+4.2*u[1])*(1.25+4.2*u[1]));
    L[0][1]=9.81;
    L[1][0]=0.002933+0.004802*u[1];
    L[1][1]=0.003623;
    L[2][0]=0.07483;
    L[2][1]=83.22;
    yy[0] = mi*(u[0]*std::cos(u[1])-(2.7+3.08*c*c)*y[0]*y[0]-mg*std::sin(y[1]));
    yy[1] = (1.0/(60000*y[0]))*(u[0]*std::sin(u[1])+68.6*c*y[0]*y[0]-mg*std::cos(y[1]));
    yy[2] = y[0]*std::sin(y[1]);
    /* to account for input disturbances */
    yy[3] = L[0][0]*y[3]+L[0][1]*y[4]+w[0]; /* L[0][2]=0 */
    yy[4] = L[1][0]*y[3]+L[1][1]*y[4]+w[1]; /* L[1][2]=0 */
    yy[5] = L[2][0]*y[3]+L[2][1]*y[4]+w[2]; /* L[2][2]=0 */
  };
  //ignore = dis.get_out_of_domain();
  //if(ignore==false)    
  scots::runge_kutta_fixed4(rhs,y,u,2*state_dim,tau,10);

  if(dis.get_intersection_check()==true){
    neigbour=true;
  }
};

 /* transition function of symbolic model */
  scots::TransitionFunction tf_old, tf_new, tf_standard,tf_new_com;

  std::cout << "Computing the transition function: " << std::endl;
  tt.tic();
  abs.compute_gb(tf_old, rs_post);
  tt.toc();
  // if(!getrusage(RUSAGE_SELF, &usage))
  //   std::cout << "Memory per transition: " << usage.ru_maxrss/(double)tf.get_no_transitions() << std::endl;
  // std::cout << "Number of transitions: " << tf.get_no_transitions() << std::endl;

   dis.update_disturbance(w_2, w2_lb, w2_ub);
  //  double max_1=(1.0/60000) * (i_ub[0]-(2.7+3.08*std::pow(1.25+4.2*i_lb[1],2))*std::pow(s_lb[0],2)+ 60000.0*9.81);
  //  double max_2=(1/(60000*s_lb[0])) * (i_ub[0] + (68.6*(1.25+4.2*i_ub[1]))*std::pow(s_ub[0],2) + 60000.0*9.81);
  //  double max_3=s_ub[0];
  // state_type max_dynamic = {{ max_1, max_2 , max_3}};
  // state_type distance = dis.get_maxdistance(max_dynamic,tau);

   std::cout << "Computing the stardard transition function globally (after distrubance changes): " << std::endl;
  tt.tic();
  abs.compute_gb(tf_standard,rs_post);
  
  if(!getrusage(RUSAGE_SELF, &usage))
    std::cout << "Memory per transition: " << usage.ru_maxrss/(double)tf_standard.get_no_transitions() << std::endl;
  std::cout << "Number of transitions: " << tf_standard.get_no_transitions() << std::endl;
  tt.toc();

  std::cout << "Computing the new transition function locally (after distrubance changes): " << std::endl;
  tt.tic();
  abs.recompute_gb(tf_new,tf_old, tf_standard, w2_lb, w2_ub, rs_repost);
 
   std::cout << "Number of new transitions: " << tf_new.get_no_transitions() << std::endl;
  tt.toc();

  //  std::cout << "Computing the new transition function locally (after distrubance changes): " << std::endl;
  // tt.tic();
  // abs.recompute_mr(tf_new_com,tf_old, distance, w2_lb, w2_ub, rs_post);
 
  //  std::cout << "Number of new transitions: " << tf_new_com.get_no_transitions() << std::endl;
  // tt.toc();
  /* define target set */
  auto target = [&s_eta, &z, &ss](const scots::abs_type& abs_state) {
    state_type t_lb = {{63,-3*M_PI/180,0}};
    state_type t_ub = {{75,0,2.5}};
    state_type c_lb;
    state_type c_ub;
    /* center of cell associated with abs_state is stored in x */
    state_type x;
    ss.itox(abs_state,x);
    /* hyper-interval of the quantizer symbol with perturbation */
    for(int i=0; i<state_dim; i++) {
      c_lb[i] = x[i]-s_eta[i]/2.0-z[i];
      c_ub[i] = x[i]+s_eta[i]/2.0+z[i];
    }
    if( t_lb[0]<=c_lb[0] && c_ub[0]<=t_ub[0] &&
        t_lb[1]<=c_lb[1] && c_ub[1]<=t_ub[1] &&
        t_lb[2]<=c_lb[2] && c_ub[2]<=t_ub[2]) {
      if(-0.91<=(x[0]*std::sin(x[1])-s_eta[0]/2.0-z[0]-(c_ub[0])*(s_eta[1]/2.0-z[1]))) {
        return true;
      }
    }
    return false;
  };
  /* write grid point IDs with uniform grid information to file */
  write_to_file(ss,target,"target");
 
  std::cout << "\nSynthesis: " << std::endl;
  tt.tic();
  scots::WinningDomain win=scots::solve_reachability_game(tf_new,target);
  tt.toc();
  std::cout << "Winning domain size: " << win.get_size() << std::endl;

  std::cout << "\nWrite controller to controller.scs \n";
  if(write_to_file(scots::StaticController(ss,is,std::move(win)),"controller"))
    std::cout << "Done. \n";

  return 1;
}
