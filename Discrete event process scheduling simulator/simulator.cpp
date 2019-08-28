/*
	OS Assignment 2
	Authors: Naman Jain, Kumar Prithvi Mishra, Ayush Chaurasia
*/




#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
using namespace std;

template<class T> ostream& operator<<(ostream &os, vector<T> V) {
 os << "[ "; for(auto v : V) os << v << " "; return os << "]";}
template<class L, class R> ostream& operator<<(ostream &os, pair<L,R> P) {
	return os << "(" << P.first << "," << P.second << ")";}

#define TRACE
#ifdef TRACE
#define trace(...) __f(#__VA_ARGS__, __VA_ARGS__)
template <typename Arg1>
void __f(const char* name, Arg1&& arg1){
	cout << name << " : " << arg1 << std::endl;
}
template <typename Arg1, typename... Args>
void __f(const char* names, Arg1&& arg1, Args&&... args){
	const char* comma = strchr(names + 1, ',');cout.write(names, comma - names) << " : " << arg1<<" | ";__f(comma+1, args...);
}
#else
#define trace(...) 1
#endif


#define ll long long
#define ld long double
#define vll vector<ll>
#define pll pair<ll,ll>
#define vpll vector<pll>
#define I insert 
#define pb push_back
#define F first
#define S second
#define endl "\n"


ld EPS = 1e-9;
inline bool eq(ld a,ld b) {return fabs(a-b)<EPS;}
inline bool lt(ld a,ld b) {return a+EPS<b;}
inline bool gt(ld a,ld b) {return a>b+EPS;}
inline bool le(ld a,ld b) {return lt(a,b)||eq(a,b);}
inline bool ge(ld a,ld b) {return gt(a,b)||eq(a,b);}



ld get_expo(ld par){  // exp(a)
	int seed = chrono::system_clock::now().time_since_epoch().count();
  	default_random_engine generator (seed);
	exponential_distribution<ld> distribution(par);
	return distribution(generator);
}

ll nop = 0; // number of processes

struct process{
	ll pid;ld tg,tc;
	process(ll pid, ld tg, ld tc) : pid(pid), tg(tg), tc(tc) {}
};

struct schedule{
	ll pid;ld st,en;
	schedule(ll pid, ld st, ld en) : pid(pid), st(st), en(en) {}
};

process get_new_process(ld gpar, ld cpar){
	ll pid = ++nop;
	ld tg =  get_expo(gpar);
	ld tc = get_expo(cpar);
	tc+=2.0;
	// trace(tg,tc);
	return process(pid,tg,tc); 
}


// void print_table(vector<schedule> sch)
// {
//     int i;
//     int n = sch.size();
//     puts("+-----+------------+--------------+-----------------+");
//     puts("| PID | Burst Time | Waiting Time | Turnaround Time |");
//     puts("+-----+------------+--------------+-----------------+");
//     for(i=0; i<n; i++) {
//         printf("| %2d  |     %2d     |      %2d      |        %2d       |\n"
//                , sch[i].pid, sch[i].en-sch[i].st, sch[i].st,sch[i].en );
//         puts("+-----+------------+--------------+-----------------+");
//     }
// }

void print_gannt_chart(vector<schedule> sch){
    int i, j;
    int n = sch.size();
    // print top bar
    cout<<" ";
    for(i=0; i<n; i++) {
        for(j=0; j<(sch[i].en-sch[i].st); j++) printf("--");
        cout<<" ";
    }
    printf("\n|");
    // printing process id in the middle
    for(i=0; i<n; i++) {
        for(j=0; j<(sch[i].en-sch[i].st) - 1; j++) cout<<" ";
        printf("P%lld", sch[i].pid);
        for(j=0; j<(sch[i].en-sch[i].st) - 1; j++) cout<<" ";
        printf("|");
    }
    cout<<"\n ";
    // printing bottom bar
    for(i=0; i<n; i++) {
        for(j=0; j<(sch[i].en-sch[i].st); j++) printf("--");
        cout<<" ";
    }
    cout<<"\n";
    // printing the time line
    printf("0");

    for(i=0; i<n; i++) {
        for(j=0; j<(sch[i].en-sch[i].st); j++) cout<<"  ";
        cout<<"\b\b\b";
        if(sch[i].en > 9) cout<<"\b"; // backspace : remove 1 space
        cout<<sch[i].en;
    }
    cout<<"\n";
}
// void print_gannt_chart(vector<schedule> sch){

// }
void first_come_first_serve(vector<process> pr){
	vector<schedule> sch;
	ld cur = 0;	
	for(auto z:pr){
		ld st = max(cur,z.tg);
		ld en = st+z.tc;cur=en;
		sch.pb(schedule(z.pid,st,en));
	}
	for(auto z:sch){
		trace(z.pid,z.st,z.en);
	}
	print_gannt_chart(sch);
}


void shortest_job_first(vector<process> pr){
	priority_queue<pll> pq;
	ld cend=0;
	vector<schedule> sch;
	ll sz = pr.size();
	for(ll i=0;i<sz;i++){
		auto z = pr[i];
		if(z.tg < cend) pq.push({z.tc,i});
		else{
			if(pq.empty()){
				sch.pb(schedule(z.pid,z.tg,z.tg+z.tc));
				cend = z.tg+z.tc;
			}
			else{
				auto p1 = pq.top();pq.pop();
				auto cp = pr[p1.S];
				ld st = max(cp.tg,cend);
				sch.pb(schedule(cp.pid,st,st+cp.tc));
				cend = st+cp.tc;
				i--;
			}
		}
	}
	while(!pq.empty()){
		auto p1 = pq.top();pq.pop();
		auto cp = pr[p1.S];
		ld st = max(cp.tg,cend);
		sch.pb(schedule(cp.pid,st,st+cp.tc));
		cend = st+cp.tc;
	}
	for(auto z:sch){
		trace(z.pid,z.st,z.en);
	}
	print_gannt_chart(sch);
}

#define plll pair<ld,pll>   
#define pld pair<ld,ll>
#define vld vector<ld>

void shortest_remaining_time(vector<process> pr){
	ll len = pr.size();
	priority_queue< plll,vector<plll>, greater<plll> > events; // {time, {type, pid } }
	// type-0 -> new arrival type-1-> end;
	priority_queue<pld,vector<pld>, greater<pld> > remaining_times;
	ll i,j,k;
	vld rem_time(len+1,-1);
	for(auto z:pr){
		events.push({z.tg,{0,z.pid}});
		rem_time[z.pid] = z.tc;
	}

	ll curp = -1;ld curst = -1;

	vector<schedule> sch;

	while(!events.empty()){
		auto cur_event = events.top(); events.pop();
		ld cur_time = cur_event.F;
		ll event_pid = cur_event.S.S; 
		ll event_type = cur_event.S.F;
		// trace(cur_time,event_pid,event_type);

		if(cur_event.S.F == 0){
			ld rem_new = rem_time[event_pid];
			// if(event_pid==4){
			// 	trace(curst,curp);
			// }
			if(curp!=-1){
				ld rem_ongoing = (rem_time[curp] - (cur_time-curst));
				if(le(rem_ongoing, rem_new)){
					remaining_times.push({rem_new,event_pid});
				}
				else{	
					rem_time[curp] = rem_ongoing;
					remaining_times.push({rem_ongoing,curp});
					
					schedule news(curp,curst,cur_time);
					sch.pb(news);
					
					curp = event_pid;
					curst = cur_time;
					ld new_end = curst + rem_new;
					events.push({new_end,{1,event_pid}});


				}	
			}
			else{
				curp = event_pid;
				curst = cur_time;
				ld new_end = curst + rem_new;
				events.push({new_end,{1,event_pid}});
			}
		}
		else{
			if((curp == event_pid) && eq(curst+rem_time[event_pid] , cur_time)){
				rem_time[curp]=0;

				schedule news(curp,curst,cur_time);
				sch.pb(news);

				curp=-1;curst=-1;
				while(!remaining_times.empty()){
					auto new_pr = remaining_times.top();
					remaining_times.pop();
					// trace(new_pr.F,new_pr.S);
					if(!eq(rem_time[new_pr.S], new_pr.F))continue;
					curp = new_pr.S;
					curst = cur_time;
					ld new_end = curst + new_pr.F;
					events.push({new_end,{1,curp}});
					break;
				}
			}
		}

	}

	cout<<"\n------------------------------\n";
	for(auto z:sch){
		trace(z.pid,z.st,z.en);
	}

	print_gannt_chart(sch);
}

ld INF = 1e16;

void round_robin(vector<process> pr, ld time_slice){
	queue<ll> Q;
	ll ind = 0;
	ll len = pr.size();
	// ll curp = 0;
	// ll curst = pr[0].tg;
	vld rem_time(len+1);
	// rem_time[0] = min(pr[0].tc-time_slice,(ld)0);
	// ld nx = min(time_slice,pr[0].tc);

	ld nx=-1;
	ll curp;

	vector<schedule> sch;
	while((!Q.empty()) || (ind<len) || nx>0){
		// trace(nx,ind);
		if(nx<0){
			// trace(ind);
			auto newp = pr[ind++];
			rem_time[newp.pid] = newp.tc;
			curp = newp.pid;
			ld todo = min(rem_time[curp], time_slice);
			nx = todo + newp.tg;
			rem_time[curp]-=todo;

			schedule add(curp,newp.tg,nx); 
			sch.pb(add);
		}
		else{
			while(ind<len && le(pr[ind].tg,nx)){
				auto newp = pr[ind++];
				rem_time[newp.pid] = newp.tc;
				Q.push(newp.pid);
			}
			if(gt(rem_time[curp],0)){
				Q.push(curp);
			}
			ld old_nx = nx;
			nx=-1;

			if(!Q.empty()){
				auto nxt =Q.front();Q.pop();
				// trace(nxt,rem_time[nxt]);
				curp = nxt;
				ld todo = min(rem_time[curp], time_slice);
				ld nx_new = todo + old_nx;
				rem_time[curp]-=todo;

				schedule add(curp,old_nx,nx_new); 
				sch.pb(add);

				nx = nx_new;
			}
		}
	}	

	for(auto z:sch){
		trace(z.pid,z.st,z.en);
	}

	print_gannt_chart(sch);
}

void mlfq(vector<process> pr, vector<ld> time_slices){
	queue<ll> Q[3];
	ll ind = 0;
	ll len = pr.size();
	vld rem_time(len+1);
	ld nx=-1;
	ll curp;
	vector<schedule> sch;
	vll priorities(len+1);
	bool qem = 0;
	ll i;
	while((qem) || (ind<len) || nx>0){
		// trace(nx,ind,);
		if(nx<0){
			// trace(ind);
			auto newp = pr[ind++];
			rem_time[newp.pid] = newp.tc;
			curp = newp.pid;
			priorities[curp] = 0;
			ld todo = min(rem_time[curp], time_slices[0]);
			nx = todo + newp.tg;
			rem_time[curp]-=todo;
			priorities[curp]++;


			schedule add(curp,newp.tg,nx); 
			sch.pb(add);
		}
		else{
			while(ind<len && le(pr[ind].tg,nx)){
				auto newp = pr[ind++];
				rem_time[newp.pid] = newp.tc;
				priorities[newp.pid] = 0;
				Q[0].push(newp.pid);
			}
			if(gt(rem_time[curp],0)){
				Q[priorities[curp]].push(curp);
				priorities[curp] = min(priorities[curp]+1,2ll);
			}

			ld old_nx = nx;
			nx=-1;
			for(i=0;i<3;i++){
				if(!Q[i].empty()){
					auto nxt =Q[i].front();Q[i].pop();
					// trace(nxt,rem_time[nxt]);
					curp = nxt;
					ld todo = min(rem_time[curp], time_slices[i]);
					ld nx_new = todo + old_nx;
					rem_time[curp]-=todo;
					priorities[curp] = min(priorities[curp]+1,2ll);
					
					schedule add(curp,old_nx,nx_new); 
					sch.pb(add);

					nx = nx_new;
					break;
				}
			}
		}
		qem=0;
		for(i=0;i<3;i++)if(!Q[i].empty())qem=1;
	}	

	for(auto z:sch){
		trace(z.pid,z.st,z.en);
	}

	print_gannt_chart(sch);

}


int main(){
	// ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
	// cout<<"fnaijfjanof";
	// cout<<flush;
	// cout<<"\b\b\b";
	// cout<<flush;
	// cout<<"bfadfnak";
	// return 0;
	cout<<fixed<<setprecision(2);
	ll n;ld gpar,cpar;cin>>n>>gpar>>cpar;

	n=3; gpar=0.1; cpar=0.05;

	vector<process> pr;
	ll i;
	ld cur = 0;
	trace(n,gpar,cpar);
	for(i=0;i<n;i++){
		auto p = get_new_process(gpar,cpar);
		p.tg+=cur;
		cur=p.tg;
		pr.pb(p);
		trace(p.pid,p.tg,p.tc,cur);
	}
	cout<<"------------------------------------\n\n";
	cout<<"\n\n---------First Come First Serve ----------\n\n";
	first_come_first_serve(pr);

	cout<<"\n\n---------Shortest Job First ----------\n\n";
	shortest_job_first(pr);
	
	cout<<"\n\n---------Shortest Remaining Time First ----------\n\n";
	shortest_remaining_time(pr);

	cout<<"\n\n---------Round Robin ----------\n\n";
	round_robin(pr,3.0);

	vector<ld> ts;ts.pb(3);ts.pb(6);ts.pb(9);
	cout<<"\n\n---------Multi-Level Feedback Queue ----------\n\n";
	mlfq(pr,ts);
}
