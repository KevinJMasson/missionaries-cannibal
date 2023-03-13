//Made by Kevin Masson
//February 4 2022
//For CPSC3750
//Assignment 1
//This program simulates the missionaries and cannibals problem and uses a
//depth first search to find a solution to it.

#include<iostream>
#include<vector>
#include<map>
#include<utility>

using namespace std;

class state {
public:
  state(int a, int b, int c, int d, int e,int f) :
    lm(a),lc(b),rm(c),rc(d),boat(e),num(f){};

  //make attributes for left/right missionaries, left/right
  //cannibals, the boat and the node number (for recreating the path)
  int lm,lc,rm,rc,boat,num;

  //Each state will have a vector of connected states that are viable
  //to progress to the final state
  vector<state*> children;
};

void genconnections(vector<state*>);
state copy(const state*,int, int);
bool cmp(const state, const state*);
bool dfs(state*, map<int,bool>&, vector<int>&);

int main() {
  bool check = false;

  //generates all valid states
  vector<state*> spaces;
  state* s1 = new state(0,0,3,3,0,0);
  spaces.push_back(s1);
  state* s2 = new state(0,2,3,1,1,1);
  spaces.push_back(s2);
  state* s3 = new state(1,1,2,2,1,2);
  spaces.push_back(s3);
  state* s4 = new state(0,1,3,2,1,3);
  spaces.push_back(s4);
  state* s5 = new state(0,1,3,2,0,4);
  spaces.push_back(s5);
  state* s6 = new state(0,3,3,0,1,5);
  spaces.push_back(s6);
  state* s7 = new state(0,2,3,1,0,6);
  spaces.push_back(s7);
  state* s8 = new state(2,2,1,1,1,7);
  spaces.push_back(s8);
  state* s9 = new state(1,1,2,2,0,8);
  spaces.push_back(s9);
  state* s10 = new state(3,1,0,2,1,9);
  spaces.push_back(s10);
  state* s11 = new state(3,0,0,3,0,10);
  spaces.push_back(s11);
  state* s12 = new state(3,2,0,1,1,11);
  spaces.push_back(s12);
  state* s13 = new state(3,2,0,1,0,12);
  spaces.push_back(s13);
  state* s14 = new state(2,2,1,1,0,13);
  spaces.push_back(s14);
  state* s15 = new state(3,1,0,2,0,14);
  spaces.push_back(s15);
  state* s16 = new state(3,3,0,0,1,15);
  spaces.push_back(s16);

  //generates the connections between spaces
  genconnections(spaces);

  //set up a map to check which nodes we have visited
  map<int, bool> visited;
  for (int i = 0; i<16;i++){
    visited.insert(pair<int,bool>(i,false));
  }

  //set up a vector of the final path
  vector<int> path;

  cout << "DFS starting\n";
  check = dfs(s1,visited, path);

  //temporary variables for the current m/c count to make writing easier
  int tlm,tlc,trm,trc;

  //if a solution is found then it will output the steps to the solution
  if (check == true) {
    cout << "Solution found\n";
    cout << "Steps to solution:\n";
    for (unsigned int j = path.size();j>0;j--) {
      for (unsigned int i = 0; i<spaces.size();i++){
        if (spaces.at(i)->num == path.at(j-1)){
          tlm = spaces.at(i)->lm;
          tlc = spaces.at(i)->lc;
          trm = spaces.at(i)->rm;
          trc = spaces.at(i)->rc;
          cout << "[";
          if (tlm!=0)
            for (int i=0;i<tlm;i++) {
              cout << 'M';
            }
          if(tlc!=0)
            for (int i=0;i<tlc;i++) {
              cout << 'C';
            }
        cout << "]";
        if (spaces.at(i)->boat == 1)
          cout << "B     ";
        else
          cout << "     B";
        cout << "[";
        if (trm!=0)
          for (int i=0;i<trm;i++) {
            cout << 'M';
          }
        if (trc!=0)
          for (int i = 0;i<trc;i++) {
            cout << 'C';
          }
        cout << "]" << endl;
        break;
      }
    }
  }
  } else
    cout << "no solution found\n";
  return 0;
}

//depth first search, checking if a state has been checked already
bool dfs(state* s, map<int,bool> &v, vector<int> &p) {
  unsigned int i = 0;
  bool check = false;
  if (v.find(s->num)->second == false)
    v.find(s->num)->second = true;
  else
    return false;

  if (s->lm + s->lc == 6) {
    p.push_back(s->num);
    return true;
  }
  else {
    while (i < s->children.size() && check == false) {
      check = dfs(s->children.at(i),v,p);
      if (check) {
        p.push_back(s->num);
        return true;
      }
      i++;
    }
  }
  return false;

}

//connects all the nodes in the state space to what their next possible state
//could be
void genconnections(vector<state*> s) {
  for (unsigned int i = 0;i<s.size();i++) {
      for (int j = 0;j<5;j++) {
        if(s.at(i)->boat == 0) {
        switch(j) {
          case 0:
          if (s.at(i)->rm - 1 >= s.at(i)->rc || s.at(i)->rm-1 == 0) {
            for (unsigned int k = i+1; k<s.size();k++){
              if (cmp(copy(s.at(i),0,1), s.at(k))) {
                s.at(i)->children.push_back(s.at(k));
            }
          }
        }
          break;

          case 1:
          if (s.at(i)->rm - 2 >= s.at(i)->rc|| s.at(i)->rm-2 == 0) {
            for (unsigned int k=i+1; k<s.size();k++){
              if (cmp(copy(s.at(i),1,1), s.at(k))) {
                s.at(i)->children.push_back(s.at(k));
            }
            }
          }
          break;

          case 2:
          if (s.at(i)->rc - 1 >= 0) {
          for (unsigned int k = i+1; k<s.size();k++){
            if (cmp(copy(s.at(i),2,1), s.at(k)) == true) {
              s.at(i)->children.push_back(s.at(k));
            }
          }
        }
          break;

          case 3:
          if (s.at(i)->rc - 2 >= 0) {
          for (unsigned int k = i+1; k<s.size();k++){
            if (cmp(copy(s.at(i),3,1), s.at(k))) {
              s.at(i)->children.push_back(s.at(k));
            }
            }
          }
          break;

          case 4:
          if (s.at(i)->rm - 1 >= 0 && s.at(i)->rc - 1 >= 0) {
          for (unsigned int k = i+1; k<s.size();k++){
            if (cmp(copy(s.at(i),4,1), s.at(k))){
              s.at(i)->children.push_back(s.at(k));
            }
          }
        }
          break;
        }
      } else {
        switch(j) {
          case 0:
          if (s.at(i)->lm - 1 >= s.at(i)->lc|| s.at(i)->lm-1 == 0){
          for (unsigned int k = i+1; k<s.size();k++){
            if (cmp(copy(s.at(i),5,0), s.at(k))){
              s.at(i)->children.push_back(s.at(k));
          }
            }
          }
          break;

          case 1:
          if (s.at(i)->lm -2 >= s.at(i)->lc|| s.at(i)->rm-2 == 0){
          for (unsigned int k = i+1; k<s.size();k++){
            if (cmp(copy(s.at(i),6,0), s.at(k))) {
              s.at(i)->children.push_back(s.at(k));
            }
            }
          }
          break;

          case 2:
          if (s.at(i)->lc - 1 >= 0){
          for (unsigned int k = i+1; k<s.size();k++){
            if (cmp(copy(s.at(i),7,0), s.at(k))) {
              s.at(i)->children.push_back(s.at(k));
          }
            }
          }
          break;

          case 3:
          if (s.at(i)->lc - 2 >= 0) {
          for (unsigned int k = i+1; k<s.size();k++){
            if (cmp(copy(s.at(i),8,0), s.at(k))) {
              s.at(i)->children.push_back(s.at(k));
          }
            }
          }
          break;

          case 4:
          if (s.at(i)->lm - 1 >= 0 && s.at(i)->lc - 1 >= 0) {
          for (unsigned int k = i+1; k<s.size();k++){
            if (cmp(copy(s.at(i),9,0), s.at(k))) {
              s.at(i)->children.push_back(s.at(k));
          }
            }
          }
          break;
        }
  }
}
}
}

//checks copies a state and modifies to what the state it should look for
state copy(const state* s, int i, int b) {
  switch (i){
    case 0:
      {
      state n1(s->lm+1,s->lc,s->rm-1, s->rc,b,s->num);
      return n1;
    }
      break;
    case 1:
      {
      state n2(s->lm+2,s->lc,s->rm-2, s->rc,b, s->num);
      return n2;
      }
      break;
    case 2:
      {
      state n3(s->lm,s->lc+1,s->rm, s->rc-1,b,s->num);
      return n3;
      }
      break;
    case 3:
      {
      state n4(s->lm,s->lc+2,s->rm, s->rc-2,b,s->num);
      return n4;
      }
      break;
    case 4:
      {
      state n5(s->lm+1,s->lc+1,s->rm-1, s->rc-1,b,s->num);
      return n5;
      }
      break;
    case 5:
      {
      state n6(s->lm-1,s->lc,s->rm+1, s->rc,b,s->num);
      return n6;
      break;
    }
    case 6:
      {
      state n7(s->lm-2,s->lc,s->rm+2, s->rc,b,s->num);
      return n7;
    }
      break;
    case 7:
      {
      state n8(s->lm,s->lc-1,s->rm, s->rc+1,b,s->num);
      return n8;
    }
      break;
    case 8:
    {
      state n9(s->lm,s->lc-2,s->rm, s->rc+2,b,s->num);
      return n9;
    }
      break;
    case 9:
    {
      state n10(s->lm-1,s->lc-1,s->rm+1, s->rc+1,b,s->num);
      return n10;
    }
      break;
  }

  //it shouldnt ever get to this point, but this will get rid of the warning
  //of possibly not returning anything
  state n(s->lm,s->lc,s->rm, s->rc,b,s->num);

  return n;
}

//function to compare two states for equality
bool cmp(const state l, const state* r) {
  if (l.lm == r->lm && l.lc == r->lc && l.rm == r->rm && l.rc == r->rc &&
    l.boat == r->boat) {
      return true;
    }
  else
    return false;
}
