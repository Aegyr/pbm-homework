#include "RungeKuttaSolver.h"
using namespace std;

void RungeKuttaSolver::step(const Time stepsize)  {
	/* Please fill in your code for computing a 4th order Runge-Kutta time step here.
	 * The system on which the solver operates is available in the variable "system".
	 *
	 * The system of differential equations that you will solve is of the form
	 *
	 *   y'(t) = f(y(t)),
	 *
	 * where
	 *
	 *   y(t) = (x(t), v(t)),
	 *
	 * with x(t) and v(t) containing the positions and velocities of all the
	 * particles, respectively. You can split this system into the position and
	 * velocity components, such that
	 *
	 *   x'(t) = v(t)  and
	 *   v'(t) = a(x(t)).
	 *
	 * You can evaluate a(x(t)) by setting all the particle positions in the system to
	 * the corresponding values of x(t), calling _system->computeAccelerations() and
	 * reading the accelerations back from the particles. If you want to be generic
	 * and allow for systems where the accelerations may also depend on the velocity
	 * (for example, when there is friction) or on the time (for example, when an
	 * external force changes over time), you should also set the particle velocities
	 * and the system time before calling computeAccelerations().
	 *
	 * When your computation is done, store the new x and v values in the particles.
	 * Do not forget to increment the current time of the system when you are done.
	 */

    vector<Length3D> r1, r2, r3, r4;
    vector<Velocity3D> v1, v2, v3, v4;
    vector<Acceleration3D> a1, a2, a3, a4;

    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        r1.push_back(p->position);
        v1.push_back(p->velocity);
    }
    compAcc(r1, &a1);

    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        r2.push_back(r1[i]+.5*stepsize*v1[i]);
        v2.push_back(p->velocity+.5*stepsize*a1[i]);
    }
    compAcc(r2, &a2);

    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        r3.push_back(r1[i]+.5*stepsize*v2[i]);
        v3.push_back(p->velocity+.5*stepsize*a2[i]);
    }
    compAcc(r3, &a3);

    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        r4.push_back(r1[i]+stepsize*v3[i]);
        v4.push_back(p->velocity+stepsize*a3[i]);
    }
    compAcc(r4, &a4);

    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        p->position = r1[i] + stepsize/6.0*(v1[i] + 2*v2[i] + 2*v3[i] + 2*v4[i]);
        p->velocity = p->velocity + stepsize/6.0*(a1[i] + 2*a2[i] + 2*a3[i] + 2*a4[i]);
    }

    _system->time += stepsize;
}

void RungeKuttaSolver::compAcc(vector<Length3D> r, vector<Acceleration3D>* a) {
    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        p->position = r[i];
    }
    _system->computeAccelerations();
    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        a->push_back(p->acceleration);
    }
}
