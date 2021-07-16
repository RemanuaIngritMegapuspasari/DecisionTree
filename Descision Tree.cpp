#include<iostream> #define S 50 using namespace std; int main() { int n, i; float x[S], y[S], sumX=0, sumX2=0, sumY=0, sumXY=0, a, b; /* Input */ cout<<"How many data points? "; cin>>n; cout<<"Enter data:"<< endl; for(i=1;i<=n;i++) { cout<<"x["<< i <<"] = "; cin>>x[i]; cout<<"y["<< i <<"] = "; cin>>y[i]; } /* Calculating Required Sum */ for(i=1;i<=n;i++) { sumX = sumX + x[i]; sumX2 = sumX2 + x[i]*x[i]; sumY = sumY + y[i]; sumXY = sumXY + x[i]*y[i]; } /* Calculating a and b */ b = (n*sumXY-sumX*sumY)/(n*sumX2-sumX*sumX); a = (sumY - b*sumX)/n; /* Displaying value of a and b */ cout<<"Calculated value of a is "<< a << "and b is "<< b << endl; cout<<"Equation of best fit is: y = "<< a <<" + "<< b<<"x"; return(0); } 
struct attrItem
 {
  std::vector<int>  itemNum;  //itemNum[0] = itemLine.size()
                               //itemNum[1] = decision num
     set<int>          itemLine;
 };

struct attributes
  {
  string attriName;
   vector<double> statResult;
   map<string, attrItem*> attriItem;
 }; 

vector<attributes*> statTree;
struct TreeNode 2 { 3 std::string m_sAttribute; 4 int m_iDeciNum; 5 int m_iUnDecinum; 6 std::vector<TreeNode*> m_vChildren; 7 };
ruct TreeNode 
2 {
3     std::string               m_sAttribute;
4     int                       m_iDeciNum;
5     int                       m_iUnDecinum;
6     std::vector<TreeNode*>    m_vChildren;    
7 };
　 The program source code is as follows (the program has detailed comments):

  1 #include "DecisionTree.h"
  2 
  3 int main(int argc, char* argv[]){
  4     string filename = "source.txt";
  5     DecisionTree dt ;
  6     int attr_node = 0;
  7     TreeNode* treeHead = nullptr;
  8     set<int> readLineNum;
  9     vector<int> readClumNum;
 10     int deep = 0;
 11     if (dt.pretreatment(filename, readLineNum, readClumNum) == 0)
 12     {
 13         dt.CreatTree(treeHead, dt.getStatTree(), dt.getInfos(), readLineNum, readClumNum, deep);
 14     }
 15     return 0;
 16 }
 17 /*
 18 * @function CreatTree preprocessing function, responsible for reading in data, and generating information matrix and attribute tags
 19 * @param: filename filename
 20 * @param: readLineNum can use line set
 21 * @param: readClumNum available attribute set
 22 * @return int return function execution status
 23 */
 24 int DecisionTree::pretreatment(string filename, set<int>& readLineNum, vector<int>& readClumNum)
 25 {
 26     ifstream read(filename.c_str());
 27     string itemline = "";
 28     getline(read, itemline);
 29     istringstream iss(itemline);
 30     string attr = "";
 31     while(iss >> attr)
 32     {
 33         attributes* s_attr = new attributes();
 34         s_attr->attriName = attr;
 35         //Initial property name
 36         statTree.push_back(s_attr);
 37         //Initialize attribute mapping
 38         attr_clum[attr] = attriNum;
 39         attriNum++;
 40         //Initialize the list of available attributes
 41         readClumNum.push_back(0);
 42         s_attr = nullptr;
 43     }
 44 
 45     int i  = 0;
 46     //Add specific data
 47     while(true)
 48     {
 49         getline(read, itemline);
 50         if(itemline == "" || itemline.length() <= 1)
 51         {
 52             break;
 53         }
 54         vector<string> infoline;
 55         istringstream stream(itemline);
 56         string item = "";
 57         while(stream >> item)
 58         {
 59             infoline.push_back(item);
 60         }
 61 
 62         infos.push_back(infoline);
 63         readLineNum.insert(i);
 64         i++;
 65     }
 66     read.close();
 67     return 0;
 68 }
 69 
 70 int DecisionTree::statister(vector<vector<string>>& infos, vector<attributes*>& statTree, 
 71                             set<int>& readLine, vector<int>& readClumNum)
 72 {
 73     //yes total number of rows
 74     int deciNum = 0;
 75     //Count every row
 76     set<int>::iterator iter_end = readLine.end();
 77     for (set<int>::iterator line_iter = readLine.begin(); line_iter != iter_end; ++line_iter)
 78     {
 79         bool decisLine = false;
 80         if (infos[*line_iter][attriNum - 1] == "yes")
 81         {
 82             decisLine = true;
 83             deciNum++; 
 84         }
 85         //If the column is not locked and is an attribute column, perform statistics
 86         for (int i = 0; i < attriNum - 1; i++)
 87         {
 88             if (readClumNum[i] == 0)
 89             {
 90                 std::string tempitem = infos[*line_iter][i];
 91                 auto map_iter = statTree[i]->attriItem.find(tempitem);
 92                 //could not find it
 93                 if (map_iter == (statTree[i]->attriItem).end())
 94                 {
 95                     //New
 96                     attrItem* attritem = new attrItem();
 97                     attritem->itemNum.push_back(1);
 98                     decisLine ? attritem->itemNum.push_back(1) : attritem->itemNum.push_back(0);
 99                     attritem->itemLine.insert(*line_iter);
100                     //Establish attribute name->item mapping
101                     (statTree[i]->attriItem)[tempitem] = attritem;
102                     attritem = nullptr;
103                 }
104                 else
105                 {
106                     (map_iter->second)->itemNum[0]++;
107                     (map_iter->second)->itemLine.insert(*line_iter);
108                     if(decisLine)
109                     {
110                         (map_iter->second)->itemNum[1]++;
111                     }
112                 }
113             }
114         }
115     }
116     return deciNum;
117 }
118 
119 /*
120 * @function CreatTree Recursive DFS creates and outputs decision trees
121 * @param: treeHead is the generated decision tree
122 * @param: statTree is the state tree, this tree is dynamically updated, but because DFS updates the data, there is no need to create a new state tree every time
123 * @param: infos data information
124 * @param: readLine The number of lines to be counted in the infos currently, given outside the function
125 * @param: deep determines the depth of the tree for printing
126 * @return void
127 */
128 void DecisionTree::CreatTree(TreeNode* treeHead, vector<attributes*>& statTree, vector<vector<string>>& infos, 
129                              set<int>& readLine, vector<int>& readClumNum, int deep)
130 {
131     //There are countable rows
132     if (readLine.size() != 0)
133     {
134         string treeLine = "";
135         for (int i = 0; i < deep; i++)
136         {
137             treeLine += "--";
138         }
139         //Clear other attribute subtrees and recurse
140         resetStatTree(statTree, readClumNum);
141         //Count the data in the current readLine: including which attributes and rows are counted,
142         //And generate statTree (due to a common statTree, all use references instead), and return the number of destination information
143         int deciNum = statister(getInfos(), statTree, readLine, readClumNum);
144         int lineNum = readLine.size();
145         int attr_node = compuDecisiNote(statTree, deciNum, lineNum, readClumNum);//Copy this article as a local variable
146         //The column is locked
147         readClumNum[attr_node] = 1;
148         //Build root
149         TreeNode* treeNote = new TreeNode();
150         treeNote->m_sAttribute = statTree[attr_node]->attriName;
151         treeNote->m_iDeciNum = deciNum;
152         treeNote->m_iUnDecinum = lineNum - deciNum;
153         if (treeHead == nullptr)
154         {
155             treeHead = treeNote; //root
156         }
157         else
158         {
159             treeHead->m_vChildren.push_back(treeNote); //Child node
160         }
161         cout << "node-"<< treeLine << ">" << statTree[attr_node]->attriName    << endl;
162         
163         //Recurse from child branch
164         for(map<string, attrItem*>::iterator map_iterator = statTree[attr_node]->attriItem.begin();
165             map_iterator != statTree[attr_node]->attriItem.end(); ++map_iterator)
166         {
167             //Print branch
168             int sum = map_iterator->second->itemNum[0];
169             int deci_Num = map_iterator->second->itemNum[1];
170             cout << "Branch--"<< treeLine << ">" << map_iterator->first << endl;
171             //Recursive calculation, creation
172             if (deci_Num != 0 && sum != deci_Num )
173             {
174                 //Calculate the effective number of rows
175                 set<int> newReadLineNum = map_iterator->second->itemLine;
176                 //DFS
177                 CreatTree(treeNote, statTree, infos, newReadLineNum, readClumNum, deep + 1);
178             }
179             else
180             {
181                 //Create leaf nodes
182                 TreeNode* treeEnd = new TreeNode();
183                 treeEnd->m_sAttribute = statTree[attr_node]->attriName;
184                 treeEnd->m_iDeciNum = deci_Num;
185                 treeEnd->m_iUnDecinum = sum - deci_Num;
186                 treeNote->m_vChildren.push_back(treeEnd);
187                 //Print leaves
188                 if (deci_Num == 0)
189                 {
190                     cout << "leaf---"<< treeLine << ">no" << endl;
191                 }
192                 else
193                 {
194                     cout << "leaf---"<< treeLine << ">yes" << endl;
195                 }
196             }
197         }
198         //Restore attribute column availability
199         readClumNum[attr_node] = 0;
200     }
201 }
202 /*
203 * @function compuDecisiNote calculates C4.5
204 * @param: statTree is the state tree, this tree is dynamically updated, but because DFS updates the data, there is no need to create a new state tree every time
205 * @param: data volume of deciNum Yes
206 * @param: lineNum calculates the number of lines in the set
207 * @param: readClumNum set for calculation
208 * @return int The attribute number with the most information
209 */
210 int DecisionTree::compuDecisiNote(vector<attributes*>& statTree, int deciNum, int lineNum, vector<int>& readClumNum)
211 {
212     double max_temp = 0;
213     int max_attribute = 0;
214     //The total amount of information in the yes line
215     double infoD = info_D(deciNum, lineNum);
216     for (int i = 0; i < attriNum - 1; i++)
217     {
218         if (readClumNum[i] == 0)
219         {
220             double splitInfo = 0.0;
221             //info
222             double info_temp = Info_attr(statTree[i]->attriItem, splitInfo, lineNum);
223             statTree[i]->statResult.push_back(info_temp);
224             //gain
225             double gain_temp = infoD - info_temp;
226             statTree[i]->statResult.push_back(gain_temp);
227             //split_info
228             statTree[i]->statResult.push_back(splitInfo);
229             //gain_info
230             double temp = gain_temp / splitInfo;
231             statTree[i]->statResult.push_back(temp);
232             //Get the maximum value*/
233             if (temp > max_temp)
234             {
235                 max_temp = temp;
236                 max_attribute = i;
237             }
238         }
239     }
240     return max_attribute;
241 }
242 /*
243 * @function Info_attr info_D total information
244 * @param: deciNum valid information
245 * @param: sum total information
246 * @return double ratio of total information
247 */
248 double DecisionTree::info_D(int deciNum, int sum)
249 {
250     double pi = (double)deciNum / (double)sum;
251     double result = 0.0;
252     if (pi == 1.0 || pi == 0.0)
253     {
254         return result;
255     }
256     result = pi * (log(pi) / log((double)2)) + (1 - pi)*(log(1 - pi)/log((double)2));
257     return -result;
258 }
259 /*
260 * @function Info_attr total information
261 * @param: deciNum valid information
262 * @param: sum total information
263 * @return double 
264 */
265 double DecisionTree::Info_attr(map<string, attrItem*>& attriItem, double& splitInfo, int lineNum)
266 {
267     double result = 0.0;
268     for (map<string, attrItem*>::iterator item = attriItem.begin();
269          item != attriItem.end();
270          ++item
271         )
272     {
273          double pi = (double)(item->second->itemNum[0]) / (double)lineNum;
274          splitInfo += pi * (log(pi) / log((double)2));
275          double sub_attr = info_D(item->second->itemNum[1], item->second->itemNum[0]);
276          result += pi * sub_attr;
277     }
278     splitInfo = -splitInfo;
279     return result;
280 }
281 /*
282 * @function resetStatTree clean up the status tree
283 * @param: statTree status tree
284 * @param: readClumNum attribute set to be cleaned up
285 * @return void
286 */
287 void DecisionTree::resetStatTree(vector<attributes*>& statTree, vector<int>& readClumNum)
288 {
289     for (int i = 0; i < readClumNum.size() - 1; i++)
290     {
291         if (readClumNum[i] == 0)
292         {
293             map<string, attrItem*>::iterator it_end = statTree[i]->attriItem.end();
294             for (map<string, attrItem*>::iterator it = statTree[i]->attriItem.begin();
295                 it != it_end; it++)
296             {
297                 delete it->second;
298             }
299             statTree[i]->attriItem.clear();
300             statTree[i]->statResult.clear();
301         }
302     }
303 }
