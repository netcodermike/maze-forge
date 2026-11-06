#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
struct Maze { int w,h;std::vector<int>walls; };
const int dx[]={0,1,0,-1},dy[]={-1,0,1,0};
Maze generate(int w,int h,uint32_t seed){
 if(w<2||h<2||w>80||h>80)throw std::runtime_error("Dimensions must be 2..80");
 Maze maze{w,h,std::vector<int>(w*h,15)};std::mt19937 rng(seed);std::vector<bool>seen(w*h,false);std::vector<int>stack{0};seen[0]=true;
 while(!stack.empty()){
  int cell=stack.back(),x=cell%w,y=cell/w;std::vector<int>options;
  for(int d=0;d<4;d++){int nx=x+dx[d],ny=y+dy[d];if(nx>=0&&ny>=0&&nx<w&&ny<h&&!seen[ny*w+nx])options.push_back(d);}
  if(options.empty()){stack.pop_back();continue;}
  int d=options[rng()%options.size()],next=(y+dy[d])*w+x+dx[d];maze.walls[cell]&=~(1<<d);maze.walls[next]&=~(1<<((d+2)%4));seen[next]=true;stack.push_back(next);
 }return maze;
}
std::vector<int>solve(const Maze&m){
 std::queue<int>q;q.push(0);std::vector<int>parent(m.w*m.h,-1);parent[0]=0;
 while(!q.empty()){int cell=q.front();q.pop();if(cell==m.w*m.h-1)break;for(int d=0;d<4;d++)if(!(m.walls[cell]&(1<<d))){int nx=cell%m.w+dx[d],ny=cell/m.w+dy[d];if(nx<0||ny<0||nx>=m.w||ny>=m.h)throw std::runtime_error("Invalid maze edge");int n=ny*m.w+nx;if(parent[n]==-1){parent[n]=cell;q.push(n);}}}
 std::vector<int>path;if(parent.back()==-1)return path;for(int c=m.w*m.h-1;;c=parent[c]){path.push_back(c);if(c==0)break;}std::reverse(path.begin(),path.end());return path;
}
void html(const Maze&m,const std::string&file){
 std::ofstream out(file);if(!out)throw std::runtime_error("Cannot write output");
 out<<R"HTML(<!doctype html><html lang="en"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>Maze Forge</title><style>body{background:#112334;color:#e6f2fc;font:18px system-ui;max-width:900px;margin:30px auto;padding:20px}canvas{width:100%;background:#172e43;touch-action:none}button{font:inherit;padding:12px;margin:6px;background:#9be2ba;border:0;border-radius:6px}</style><h1>Maze Forge</h1><p>Reach the pink exit. Use arrow keys, WASD, or the buttons.</p><canvas id="maze" width="900" height="650" tabindex="0" aria-label="Playable maze"></canvas><p id="status" role="status">Your move.</p><button onclick="move(0)">Up</button><button onclick="move(3)">Left</button><button onclick="move(2)">Down</button><button onclick="move(1)">Right</button><button onclick="reset()">Restart</button><button onclick="hint=true;draw()">Reveal route</button><script>)HTML";
 out<<"const w="<<m.w<<",h="<<m.h<<",walls=[";for(size_t i=0;i<m.walls.size();i++){if(i)out<<',';out<<m.walls[i];}out<<"],route=[";auto path=solve(m);for(size_t i=0;i<path.size();i++){if(i)out<<',';out<<path[i];}out<<"];";
 out<<R"HTML(let player=0,moves=0,hint=false;const canvas=document.getElementById('maze'),c=canvas.getContext('2d'),sx=900/w,sy=650/h;function draw(){c.clearRect(0,0,900,650);for(let n=0;n<w*h;n++){const x=n%w*sx,y=Math.floor(n/w)*sy;if(hint&&route.includes(n)){c.fillStyle='#627456';c.fillRect(x+2,y+2,sx-4,sy-4);}c.strokeStyle='#91aac0';c.lineWidth=2;c.beginPath();if(walls[n]&1){c.moveTo(x,y);c.lineTo(x+sx,y);}if(walls[n]&2){c.moveTo(x+sx,y);c.lineTo(x+sx,y+sy);}if(walls[n]&4){c.moveTo(x,y+sy);c.lineTo(x+sx,y+sy);}if(walls[n]&8){c.moveTo(x,y);c.lineTo(x,y+sy);}c.stroke();}for(const[n,color]of [[w*h-1,'#f58cae'],[player,'#8fe6bd']]){c.fillStyle=color;c.beginPath();c.arc(n%w*sx+sx/2,Math.floor(n/w)*sy+sy/2,Math.min(sx,sy)*.25,0,Math.PI*2);c.fill();}}function move(d){if(player===w*h-1)return;if(!(walls[player]&(1<<d))){player+=[-w,1,w,-1][d];moves++;draw();document.getElementById('status').textContent=player===w*h-1?'Escaped in '+moves+' moves!':moves+' moves';}}function reset(){player=0;moves=0;hint=false;document.getElementById('status').textContent='Your move.';draw();}document.onkeydown=e=>{if(e.target.tagName==='BUTTON')return;const d={ArrowUp:0,w:0,ArrowRight:1,d:1,ArrowDown:2,s:2,ArrowLeft:3,a:3}[e.key];if(d!==undefined){e.preventDefault();move(d);}};draw();</script></html>)HTML";
 if(!out)throw std::runtime_error("Write failed");
}
int main(int argc,char**argv){try{
 if(argc==2&&std::string(argv[1])=="--self-test"){
  for(uint32_t seed=0;seed<20;seed++){auto m=generate(15,12,seed);int openings=0;for(int v:m.walls)for(int d=0;d<4;d++)if(!(v&(1<<d)))openings++;if(openings!=2*(15*12-1)||solve(m).empty()||m.walls!=generate(15,12,seed).walls)return 1;}
  try{generate(0,5,1);return 1;}catch(const std::exception&){}std::cout<<"Maze connectivity, tree edges, and determinism passed\n";return 0;
 }
 int w=20,h=15;uint32_t seed=42;std::string output="maze.html";
 for(int i=1;i<argc;i++){std::string key=argv[i];if(key=="--help"){std::cout<<"maze [--width 20] [--height 15] [--seed 42] [--output maze.html]\n";return 0;}if(i+1>=argc)throw std::runtime_error("Missing option value");std::string value=argv[++i];if(key=="--width")w=std::stoi(value);else if(key=="--height")h=std::stoi(value);else if(key=="--seed")seed=static_cast<uint32_t>(std::stoul(value));else if(key=="--output")output=value;else throw std::runtime_error("Unknown option: "+key);}
 html(generate(w,h,seed),output);std::cout<<"Open "<<output<<" in your browser to play.\n";return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
