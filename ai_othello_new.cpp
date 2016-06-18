#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
#include<string.h>
#include<vector>
#include<stack>
#define WIDE 8
#define HIGH 8
#include "SDL/SDL.h"
using namespace std;
struct Boards
{
    int board[9][9];
};
void InitialBoard(Boards *rootb)
{
    (*rootb).board[4][4]=1;
    (*rootb).board[4][5]=-1;
    (*rootb).board[5][4]=-1;
    (*rootb).board[5][5]=1;

}
void PrintBoard(Boards rootb)
{
    printf("   A   B   C   D   E   F   G   H\n");
    for(int i=1; i<=8; i++)
    {
        if(i==1) printf(" שÝשששÞשששÞשששÞשששÞשששÞשששÞשששÞשששß\n");
        printf("%d",i);
        for(int j=1; j<=8; j++)
        {
            if(rootb.board[i][j]==0)
            {
                if(j==1) printf("שר  שר");
                else if(j!=1) printf("  שר");
            }
            else if(rootb.board[i][j]==-1)
            {
                if(j==1) printf("שר¡´שר");
                else if(j!=1) printf("¡´שר");
            }
            else if(rootb.board[i][j]==1)
            {
                if(j==1) printf("שר¡³שר");
                else if(j!=1) printf("¡³שר");
            }
        }
        printf("\n");
        if(i==8) printf(" שדשששהשששהשששהשששהשששהשששהשששהשששו\n");
        else printf(" שאשששבשששבשששבשששבשששבשששבשששבשששג\n");
    }
}

vector<pair<int,int> >ChangeDirection(Boards node,int x,int y,int turn) //turn=-1 (black),turn=1 (white)
{
    vector<pair<int,int> > directions;
    if(node.board[x][y])
    {
        return directions;
    }
    //east
    if((y<=6)&&node.board[x][y+1]==-turn)
    {
        for(int i=y+2 ; i<=8 ; i++)
        {
            if(node.board[x][i]==0) break;
            else if(node.board[x][i]==turn)
            {
                directions.push_back(make_pair<int,int>(0,x*8+i));

                break;
            }
        }
    }
    //southeast
    if((y<=6)&&(x<=6)&&(node.board[x+1][y+1]==-turn))
    {
        for(int i=0; (x+i+2<=8) && (y+i+2<=8); i++)
        {
            if(node.board[x+i+2][y+i+2]==0) break;
            else if(node.board[x+i+2][y+i+2]==turn)
            {
                directions.push_back(make_pair<int,int>(1,(x+i+2)*8+(y+i+2)));

                break;
            }
        }
    }
    //south
    if((x<=6)&&(node.board[x+1][y]==-turn))
    {
        for(int i=x+2; i<=8; i++)
        {
            if(!node.board[i][y]) break;
            else if(node.board[i][y]==turn)
            {
                directions.push_back(make_pair<int,int>(2,i*8+y));

                break;
            }
        }
    }
    //Southwest
    if((y>=3)&&(x<=6)&&node.board[x+1][y-1]==-turn)
    {
        for(int i=0; (x+i+2<=8)&&(y-i-2>=1); i++)
        {
            if(node.board[x+i+2][y-i-2]==0) break;
            else if(node.board[x+i+2][y-i-2]==turn)
            {
                directions.push_back(make_pair<int,int>(3,(x+i+2)*8+(y-i-2)));

                break;
            }
        }
    }
    //west
    if((y>=3)&&(node.board[x][y-1]==-turn))
    {
        for(int i=y-2; i>=1; i--)
        {
            if(node.board[x][i]==0) break;
            else if(node.board[x][i]==turn)
            {
                directions.push_back(make_pair<int,int>(4,x*8+i));

                break;
            }
        }
    }
    //Northwest
    if((y>=3)&&(x>=3)&&(node.board[x-1][y-1]==-turn))
    {
        for(int i=0; (x-i-2>=1)&&(y-i-2>=1); i++)
        {
            if(node.board[x-i-2][y-i-2]==0) break;
            else if(node.board[x-i-2][y-i-2]==turn)
            {
                directions.push_back(make_pair<int,int>(5,(x-i-2)*8+(y-i-2)));

                break;
            }
        }
    }
    //North
    if((x>=3)&&(node.board[x-1][y]==-turn))
    {
        for(int i=x-2; i>=1; i--)
        {
            if(node.board[i][y]==0) break;
            else if(node.board[i][y]==turn)
            {
                directions.push_back(make_pair<int,int>(6,i*8+y));
                break;
            }
        }
    }
    //Northeast
    if((y<=6)&&(x>=3)&&(node.board[x-1][y+1]==-turn))
    {
        for(int i=0; (x-i-2>=1)&&(y+i+2<=8); i++)
        {
            if(node.board[x-i-2][y+i+2]==0) break;
            else if(node.board[x-i-2][y+i+2]==turn)
            {
                directions.push_back(make_pair<int,int>(7,(x-i-2)*8+(y+i+2)));
                break;
            }
        }
    }
    return directions;

}
vector<pair<int ,vector<pair<int,int> > > > GetMoves(Boards node,int turn)
{
    vector<pair<int ,vector<pair<int,int> > > > moves;
    for(int i=1; i<=8; i++)
    {
        for(int j=1; j<=8; j++)
        {
            moves.push_back(make_pair<int,vector<pair<int,int>>>(i*8+j,ChangeDirection(node,i,j,turn)));
            if(moves.back().second.size()==0) moves.pop_back();
        }
    }
    return moves;
}
double EvaluationFunction(Boards node,int turn)
{
    int my_piece=0,opp_piece=0,my_front_piece=0,opp_front_piece=0;
    double p=0,c=0,l=0,m=0,f=0,d=0;
    //8 directions construct a square
    int dir1[]= {-1,-1,-1,1,1,1,0,0};
    int dir2[]= {1,0,-1,1,0,-1,1,-1};
    //Evaluation value for all grids
    int v[9][9]= {0,0,0,0,0,0,0,0,0,
                  0,1000, -3, 11, 8, 8, 11, -3, 1000,
                  0,-3, -7, -4, 1, 1, -4, -7, -3,
                  0,11, -4, 2, 2, 2, 2, -4, 11,
                  0,8, 1, 2, -3, -3, 2, 1, 8,
                  0,8, 1, 2, -3, -3, 2, 1, 8,
                  0,11, -4, 2, 2, 2, 2, -4, 11,
                  0,-3, -7, -4, 1, 1, -4, -7, -3,
                  0,1000, -3, 11, 8, 8, 11, -3, 1000,
                 };

    //Piece difference, frontier disks and disk squares
    for(int i=1; i<=8; i++)
    {
        for(int j=1; j<=8; j++)
        {
            //Compute piece number for each player on board
            if(node.board[i][j]==turn)
            {
                d+=v[i][j];
                my_piece+=1;
            }
            else if(node.board[i][j]==-turn)
            {
                d-=v[i][j];
                opp_piece+=1;
            }
            if(node.board[i][j]!=0)
            {
                for(int k=0; k<8; k++)
                {
                    int x=i+dir1[k];
                    int y=j+dir2[k];
                    if(x>=1 && x<=8 && y>=1 && y<=8 && node.board[x][y]==0)
                    {
                        if(node.board[i][j]==turn) my_front_piece+=1;
                        else opp_front_piece+=1;
                        break;
                    }
                }
            }
        }
    }
    if(my_piece> opp_piece)
        p=(100.0*my_piece)/(my_piece+opp_piece);
    else if(my_piece<opp_piece)
        p=0-(100.0*opp_piece)/(my_piece+opp_piece);
    else p=0;

    if(my_front_piece> opp_front_piece)
        f=(100.0*my_front_piece)/(my_front_piece+opp_front_piece);
    else if(my_front_piece<opp_front_piece)
        f=0-(100.0*opp_front_piece)/(my_front_piece+opp_front_piece);
    else f=0;
    // Corner occupancy
    my_piece=opp_piece=0;
    if(node.board[1][1]==turn) my_piece+=1;
    else if(node.board[1][1]==-turn) opp_piece+=1;
    if(node.board[1][8]==turn) my_piece+=1;
    else if(node.board[1][8]==-turn) opp_piece+=1;
    if(node.board[8][1]==turn) my_piece+=1;
    else if(node.board[8][1]==-turn) opp_piece+=1;
    if(node.board[8][8]==turn) my_piece+=1;
    else if(node.board[8][8]==-turn) opp_piece+=1;
    c=25*(my_piece-opp_piece);
    //Corner closeness
    my_piece=opp_piece=0;
    //for northwest corner
    if(node.board[1][1]==0)
    {
        if(node.board[1][2]==turn) my_piece+=1;
        else if(node.board[1][2]==-turn) opp_piece+=1;
        if(node.board[2][2]==turn) my_piece+=1;
        else if(node.board[2][2]==-turn) opp_piece+=1;
        if(node.board[2][1]==turn) my_piece+=1;
        else if(node.board[2][1]==-turn) opp_piece+=1;
    }
    //for northeast corner
    if(node.board[1][8]==0)
    {
        if(node.board[2][7]==turn) my_piece+=1;
        else if(node.board[2][7]==-turn) opp_piece+=1;
        if(node.board[2][8]==turn) my_piece+=1;
        else if(node.board[2][8]==-turn) opp_piece+=1;
        if(node.board[1][7]==turn) my_piece+=1;
        else if(node.board[1][7]==-turn) opp_piece+=1;
    }
    //for southwest corner
    if(node.board[8][1]==0)
    {
        if(node.board[7][1]==turn) my_piece+=1;
        else if(node.board[7][1]==-turn) opp_piece+=1;
        if(node.board[8][2]==turn) my_piece+=1;
        else if(node.board[8][2]==-turn) opp_piece+=1;
        if(node.board[7][2]==turn) my_piece+=1;
        else if(node.board[7][2]==-turn) opp_piece+=1;
    }
    //for southeast corner
    if(node.board[8][8]==0)
    {
        if(node.board[8][7]==turn) my_piece+=1;
        else if(node.board[8][7]==-turn) opp_piece+=1;
        if(node.board[7][8]==turn) my_piece+=1;
        else if(node.board[7][8]==-turn) opp_piece+=1;
        if(node.board[7][7]==turn) my_piece+=1;
        else if(node.board[7][7]==-turn) opp_piece+=1;
    }
    l=-12.5*(my_piece-opp_piece);
    //Mobility(flexible)
    vector<pair<int ,vector<pair<int,int> > > > mymoves=GetMoves(node,turn);
    vector<pair<int ,vector<pair<int,int> > > > oppmoves=GetMoves(node,-turn);
    my_piece=mymoves.size();
    opp_piece=oppmoves.size();

    if(my_piece>opp_piece) m=(100.0*my_piece)/(my_piece+opp_piece);
    else if(my_piece<opp_piece) m=0-(100.0*opp_piece)/(my_piece+opp_piece);
    else m=0;
    //final weighted score
    double score=25*p+50*c+20*l+70*m+10*f+50*d;
    return score;
}
void MakeMoves(Boards *node,int x,int y,int turn,vector<pair<int,int> > directions)
{
    for(auto it=directions.begin(); it!=directions.end(); it++)
    {
        int i=x,j=y;
        int ii=(*it).second/8,jj=(*it).second&7;
        if(jj==0) jj=8,ii-=1;

        while((i!=ii)||(j!=jj)) //&7=%8
        {
            (*node).board[i][j]=turn;
            if(i<ii) i++;
            else if(i>ii) i--;
            if(j<jj) j++;
            else if(j>jj) j--;
        }
    }
}
double maxdouble(double x,double y,int *ansind,int ind)
{
    if(x>=y)
    {
        return x;
    }
    else if(x<y)
    {
        (*ansind)=ind;
        return y;
    }
}
double mindouble(double x,double y,int *ansind,int ind)
{
    if(x>y)
    {
        (*ansind)=ind;
        return y;
    }
    else if(x<=y)
    {
        return x;
    }
}
pair<double,int> AlphabetaSearch(Boards node,int depth,double a,double b,int turn)
{
    vector <Boards> children;
    if(depth==0)
    {
        double grade=EvaluationFunction(node,turn);    //return score of the node
        return make_pair(grade,0);
    }
    else //construct children nodes
    {
        Boards tmp=node;
        vector<pair<int ,vector<pair<int,int> > > > nextmoves=GetMoves(node,turn);

        for(auto it=nextmoves.begin(); it!=nextmoves.end(); it++)
        {
            tmp=node;
            int tmpx=((*it).first)/8;
            int tmpy=((*it).first)%8;
            if(tmpy==0) tmpx-=1,tmpy=8;
            vector<pair<int,int> > nextdirections=(*it).second;
            MakeMoves(&tmp,tmpx,tmpy,turn,nextdirections);
            children.push_back(tmp);
        }
    }
    if(turn==1) //max's turn
    {
        int index=0,ansind=0;
        while(!children.empty())
        {
            Boards cnode=children.back();
            pair<double,int> revalue=AlphabetaSearch(cnode,depth-1,a,b,-1);
            double tmpa=maxdouble(a,revalue.first,&ansind,index);
            children.pop_back();
            if(tmpa>=b) break;
            index++;
            a=tmpa;
        }
        return make_pair(a,ansind);
    }
    else if(turn==-1) //min's turn
    {
        int index=0,ansind=0;
        while(!children.empty())
        {
            Boards cnode=children.back();
            pair<double,int> revalue=AlphabetaSearch(cnode,depth-1,a,b,1);
            double tmpb=mindouble(b,revalue.first,&ansind,index);
            children.pop_back();
            if(a>=tmpb) break;
            index++;
            b=tmpb;
        }
        return make_pair(b,ansind);
    }
}
pair<int,int> Alphabeta(Boards node,int turn,int depth)
{
    int a=0-100000000,b=100000000;
    pair<double,int> value=AlphabetaSearch(node,depth,a,b,turn);
    vector<pair<int ,vector<pair<int,int> > > > nextmoves=GetMoves(node,turn);
    int nexts=nextmoves.size()-1;
    int nextstep=nextmoves[nexts-value.second].first;
    int tmpx=nextstep/8;
    int tmpy=nextstep%8;
    if(tmpy==0) tmpx-=1,tmpy=8;
    pair<int,int> moves(tmpx,tmpy);
    return moves;

}
int GetScore(Boards node,int turn)
{
    int score=0;
    for(int i=1; i<=8; i++)
    {
        for(int j=1; j<=8; j++)
        {
            if(node.board[i][j]==turn) score++;
        }
    }
    return score;
}
int main(int argc, char* argv[])
{
    //-------------SDL implementation

    SDL_Surface *screen; // even with SDL2, we can still bring ancient code back
    SDL_Window  *window;
    SDL_Surface *image;
    SDL_Surface *hello;
    SDL_Surface *white;
    SDL_Surface *black;

    window = SDL_CreateWindow("Othello Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 560, 560, 0);
    screen = SDL_GetWindowSurface(window);
    image = SDL_LoadBMP("board.bmp");
    hello = SDL_LoadBMP("hello.bmp");
    white = SDL_LoadBMP("white.bmp");
    black = SDL_LoadBMP("black.bmp");
    SDL_BlitSurface(image, NULL, screen, NULL); // blit it to the screen
    SDL_FreeSurface(image);
    SDL_UpdateWindowSurface(window);

    //-------------start algorithm
    Boards rootb;
    memset(rootb.board,0,sizeof(rootb.board));
    InitialBoard(&rootb);   //initial root board
    int turn=-1,first=1,x,y;
    SDL_Event e;
    e.type=SDL_MOUSEMOTION;
    bool quit = false;
    while (!quit)
    {
        while(SDL_PollEvent(&e))
        {
            //----------Print board
            SDL_Rect offset;
            int offx=35,offy=35;
            for(int i=1; i<=8; i++)
            {
                offy=35;
                for(int j=1; j<=8; j++)
                {
                    if(rootb.board[i][j]==-1)
                    {
                        offset.x=i+offy;
                        offset.y=j+offx;
                        SDL_BlitSurface(black, NULL, screen, &offset);
                    }
                    else if(rootb.board[i][j]==1)
                    {
                        offset.x=i+offy;
                        offset.y=j+offx;
                        SDL_BlitSurface(white, NULL,screen, &offset);
                    }
                    offy+=63;
                }
                offx+=63;
            }
            SDL_UpdateWindowSurface(window);
            SDL_Delay(15);
           //--------------------Print board finish

            //PrintBoard(rootb);
            vector<pair<int ,vector<pair<int,int> > > > moves=GetMoves(rootb,turn);
            if(moves.size()==0)
            {
                turn=-turn;
                cout<<turn<<' '<<"I have no move"<<endl;
                moves=GetMoves(rootb,turn);
                if(moves.size()==0)
                {
                    printf("Score: %d\n",GetScore(rootb,-1));
                    break;
                }
            }
            if(moves.size()!=0)
            {
                int inx,iny;
                if(turn==-1)
                {
                    if(e.type == SDL_MOUSEMOTION)
                    {

                        SDL_GetMouseState( &inx, &iny );
                    }
                    if( e.type == SDL_MOUSEBUTTONDOWN )
                    {
                        if(inx>=35&&inx<=98) y=1;
                        else if(inx>=99&&inx<=160) y=2;
                        else if(inx>=161&&inx<=221) y=3;
                        else if(inx>=222&&inx<=287) y=4;
                        else if(inx>=288&&inx<=349) y=5;
                        else if(inx>=350&&inx<=411) y=6;
                        else if(inx>=412&&inx<=474) y=7;
                        else if(inx>=475&&inx<=533) y=8;

                        if(iny>=35&&iny<=98) x=1;
                        else if(iny>=99&&iny<=160) x=2;
                        else if(iny>=161&&iny<=221) x=3;
                        else if(iny>=222&&iny<=287) x=4;
                        else if(iny>=288&&iny<=349) x=5;
                        else if(iny>=350&&iny<=411) x=6;
                        else if(iny>=412&&iny<=474) x=7;
                        else if(iny>=475&&iny<=533) x=8;

                    }

                    for(auto it=moves.begin(); it!=moves.end(); it++)
                    {
                        if(x*8+y==((*it).first))
                        {
                            MakeMoves(&rootb,x,y,turn,(*it).second);
                            turn=-turn;
                            break;
                        }
                    }
                }
                else if(turn==1)
                {
                    pair<int ,int> nextmoves=Alphabeta(rootb,turn,7);
                    int moveform=nextmoves.first*8+nextmoves.second;
                    for(auto it=moves.begin(); it!=moves.end(); it++)
                    {
                        if(moveform==((*it).first))
                        {
                            char chy=nextmoves.second+'A'-1;
                            printf("Computer chose: %d %c\n",nextmoves.first,chy);
                            MakeMoves(&rootb,nextmoves.first,nextmoves.second,turn,(*it).second);
                            turn=-turn;
                            break;
                        }
                    }

                }
            }
            //-------------------SDL implementation
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_ESCAPE])
            {
                printf("End the GAME.\n");
                quit = true;
                break;
            }
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
