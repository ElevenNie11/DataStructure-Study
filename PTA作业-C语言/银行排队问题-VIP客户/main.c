#include <stdio.h>
#define MAXN 1005   // 最大客户数量
#define MAXK 15     // 最大窗口数量

// 客户结构体
// T:到达时间  P:办理业务耗时  VIP:是否VIP客户(1是,0否)
typedef struct{
    int T;
    int P;
    int VIP;
}Customer;

int main(){
    int N, K, v;                 // N客户总数，K窗口总数，v是VIP窗口编号
    Customer c[MAXN];            // 存放所有客户信息的数组

    // 读入N个客户数据
    scanf("%d", &N);
    for(int i = 0; i < N; i++){
        scanf("%d %d %d", &c[i].T, &c[i].P, &c[i].VIP);
    }
    // 读入窗口总数K、VIP窗口编号v
    scanf("%d %d", &K, &v);


    int freeTime[MAXK] = {0};    // freeTime[k]:窗口k什么时候空闲，初始全部0时刻可用
    int count[MAXK] = {0};       // count[k]:窗口k服务了多少个客户
    int served[MAXN] = {0};      // served[i]:标记第i个客户是否已经被服务，0未服务1已服务
    int queue[MAXN];             // 等待队列，存放客户下标
    int front = 0, back = 0;     // 队列头、尾指针，[front, back)为队列内元素
    int cur = 0;                 // cur：下一个待入队客户的下标

    long long totalWait = 0;     // 所有人总等待时间，用long long防止溢出
    int maxWait = 0;             // 单个客户最长等待时间


    // 模拟循环：还有客户没入队 或者 队列里还有人等待，就继续模拟
    while(cur < N || front < back){
        // 找到下一个事件发生的时间t（要么有人到达，要么某个窗口空闲）
        int t = 1 << 30;  // 初始设为极大值
        // 候选1：下一个客户的到达时间
        if(cur < N && c[cur].T < t){
            t = c[cur].T;
        }
        // 候选2：队列不为空时，取所有窗口最早空闲的时间
        if(front < back){
            for(int k = 0; k < K; k++){
                if(freeTime[k] < t){
                    t = freeTime[k];
                }
            }
        }

        // 将所有在t时刻及之前到达的客户，全部加入等待队列
        while(cur < N && c[cur].T <= t){
            queue[back++] = cur++;
        }

        // ========= VIP窗口优先逻辑 =========
        // 如果VIP窗口v在t时刻空闲，优先在队列里找第一个VIP客户服务
        if(freeTime[v] <= t){
            for(int j = front; j < back; j++){
                int idx = queue[j];
                if(!served[idx] && c[idx].VIP){
                    served[idx] = 1;
                    int wait = t - c[idx].T;
                    totalWait += wait;
                    if(wait > maxWait){
                        maxWait = wait;
                    }
                    freeTime[v] = t + c[idx].P; // 更新VIP窗口的空闲时刻
                    count[v]++;
                    break; // 一次只服务一个VIP客户
                }
            }
        }

        // ========= 普通窗口处理，按窗口编号从小到大 =========
        // 依次检查每个窗口，如果窗口空闲，服务队首客户
        for(int k = 0; k < K; k++){
            if(front >= back) break; // 队列为空，直接退出

            if(freeTime[k] > t) continue; // 当前窗口还没空闲，跳过

            // 队头指针跳过已经被服务的客户（可能是被VIP窗口提前抓走）
            while(front < back && served[queue[front]]){
                front++;
            }
            if(front >= back) break;

            // 取出队首客户进行服务
            int idx = queue[front];
            served[idx] = 1;
            int wait = t - c[idx].T;
            totalWait += wait;
            if(wait > maxWait){
                maxWait = wait;
            }
            freeTime[k] = t + c[idx].P;
            count[k]++;
            front++;
        }
    }

    // 所有业务结束，找到全部窗口里最晚结束的时间
    int finish = 0;
    for(int k = 0; k < K; k++){
        if(freeTime[k] > finish){
            finish = freeTime[k];
        }
    }

    // 输出：平均等待时间(保留1位小数)，最长等待时间，全部业务结束时刻
    printf("%.1f %d %d\n", (double)totalWait / N, maxWait, finish);
    // 依次输出每个窗口服务的客户数量
    for(int k = 0; k < K; k++){
        if (k) printf(" ");
        printf("%d", count[k]);
    }
    printf("\n");

    return 0;
}
