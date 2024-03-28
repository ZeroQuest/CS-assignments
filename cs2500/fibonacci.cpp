#include <iostream>
#include <string>


int[2][2] naive_matrix_mult(int S[2][2], int P[2][2]);

int main()
{
  int input = 0;

  int S[2][2] = {{0, 1}, {1, 1}};
  int P[2][2] = {{0, 1}, {1, 1}};

  std::cin>>input;

  for(int i=0; i<input; i++)
  {
    S = naive_matrix_mult(S[2][2], P[2][2]);
    std::cout<<S;
  }

  return 0;
}

int[2][2] naive_matrix_mult(int S[2][2], int P[2][2])
{
  int a = S[0];
  int b = S[0].std::size();
  int g = S[0];
  int h = S[0].std::size();


  int Q[a][h] = 0;

  for(int m=0; m<a; m++)
  {
    for(int r=0; r<h; r++)
    {
      int Q[m][r] = 0;
      for(int k=0; k<g; k++)
      {
        Q[m][r] += S[m][k] * P[k][r];
      }
    }
  }
  return Q;
}
