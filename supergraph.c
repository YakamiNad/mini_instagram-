#include "supergraph.h"
#include<pthread.h>
#include<stdio.h>
#include <stdbool.h>
#include<math.h>
#include <stdlib.h>
#include <limits.h>
typedef struct Queue queu;

struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
};

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    // printf("%d enqueued to queue\n", item);s
}

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

/************
1.So far,i tried to implemented the first function with launching more threads into play.But as i have posted on the Ed just 
before,the valgrind error debug tool can't tell me which line that my error occur,which drive me crazy for debugging for a whole day.

2.For the second function,i choose to make it sequential as i might have the same problem with the first function.

3.As i have checked that my find_origin function can work perfectly and i have also passed the testcase for that 
function as well as testcases i made myself,So I comment the first function which could influence the sumbission on ed.

4.Once i find out the problem in the first function,i should be able to  finish the second function quickly using the parallel calculations.

5.I use several testcase for myself and i stick the file inside the ed.
***********/
int cursor=0;
int global;
query_helper* engine_setup(size_t n_processors) {
    /***********************(used for optimazation solution)
	// query_helper a;
	// a.process=n_processors;
	// return &a;

    *************/
    return NULL;
}



// helper recursive function
// This section of code is used for recursively go through the general tree and add correspond element
// into the result.
int recurse_numberofcount(int id,post* posts,int count_numbers){
    if(posts[id].n_reposted==0){
            return 1;
    }
     count_numbers=0;
    for(int i=0;i<posts[id].n_reposted;i++){
        count_numbers=count_numbers+recurse_numberofcount(posts[id].reposted_idxs[i],posts,count_numbers);
    }
    count_numbers=count_numbers+1;
    return count_numbers;
}


   void recurse_main(post** return_posts,post* posts,uint64_t id){    
    if(posts[id].n_reposted==0){ 
        return_posts[cursor]=&posts[id];
        cursor++;       }           
    if(posts[id].n_reposted!=0){        
        return_posts[cursor]=&posts[id];
        cursor++;}   
    for(int i=0;i<posts[id].n_reposted;i++){       
        recurse_main(return_posts,posts,posts[id].reposted_idxs[i]);      }
    return;
}






// result* recurse_findallrepost(int a,post* posts,int count,result* re,int cursor,post** el){ 

    
//     if(posts[a].n_reposted==0){
//          re->n_elements=re->n_elements+posts[a].n_reposted;
//          el=(post**)realloc(re->elements,sizeof(post*)*(re->n_elements));
//          *el[count]=posts[a].
         
//     }
//     for(int i=0;i<posts[a].n_reposted;i++)
//     {
//          //el represents element


//         re=recurse_findallrepost(posts[a].reposted_idxs[i],posts,count,re,cursor+posts[a].n_reposted,el);

//     }
//     re->elements=(void**)el;

//     return re;


// }
//
// This section will be the worked function for the threads.

// To declare first,arg is the struct i used to pass the parameters,which contains the whole posts
// and starting psd_id for each recusive function.

//
// void* handle(void* args){   
//     arg temp=*(arg*) args;          //case the void* arg
//     int index=temp.index;           
//     post* posts_array=temp.pps;
//     result* return_heap=(result*)malloc(sizeof(result));
//     return_heap->n_elements=1;
//     post** el=(post**)malloc(sizeof(post*)*1);
//     return_heap->elements=(void**)el;
//     *el[0]=posts_array[index];
//     return_heap=recurse(index,posts_array,0,return_heap,0,el);   //pass to the recursive function
//     return return_heap;    //this only returns a part of the result,which i will use join to catch the argu here.
// }


/***********************************
for the below section
1.I used a for loop to find the post that is post_id,and get all the reposted_idxs.
2.After that,i initialise all the arg agruments to be passed into the thread worker function.To be 
percise,the arguments will be the index in teh reposted_idxs corrspondingly.
3.So i launch threads(equal the number of reposted_idxs of the post_id at the very begining) to perform
different recursive function on the graph tree.
4.i waited for all the threads and stick result together to return the final_result,which would be the answer.




*****************************************/
result* find_all_reposts(post* posts, size_t count, uint64_t post_id, query_helper* helper){
    if(posts==NULL){
        return NULL;
    }
    
    int temp;
    int flag=0;
     // printf("%ld\n",count);
        for(int i=0;i<count;i++){
            if(posts[i].pst_id==post_id){
               temp=i;
                flag=1;
                break;
            }     
        }
    if(flag==0){
        return NULL;
    }
   
     int number=recurse_numberofcount(temp,posts,0);
    
    if(number==1){
       
         result* return_heap=(result*)malloc(sizeof(result));
        return_heap->n_elements=number;
        post** el=(post**)malloc(sizeof(post*)*number);
       
        el[0]=&posts[temp];
        return_heap->elements=(void**)el;
        return return_heap;
    }
    
     result* return_heap=(result*)malloc(sizeof(result));
    return_heap->n_elements=number;
    post** el=(post**)malloc(sizeof(post*)*number);
    
    recurse_main(el,posts,temp);

  
    
    return_heap->elements=(void**)el;
    
   
 
    return return_heap;
    
   
}




/*********helper_original function is used to check whether a post_id will be contained in
other posts's reposted_idxs.If yes,return that post_id so that can we can recursively go through 
this function.And i specify -1(as the negative value wont be confused wtih the psd_id value ),
once i detected if there is a -1 return,which means i cant find any post_reposted_idxs containg 
that post,which means i have reached the original post.So in the find_original function,once i 
found -1 returned,that means the previous one that stored in var updated_temp is the original post
. 

*******/


int helper_original(int id,post* posts,size_t count){
    for(int i=0;i<count;i++){
        for(int k=0;k<posts[i].n_reposted;k++){ 
            if(posts[i].reposted_idxs[k]==id){
    return i;//i refers to the reposted_idxs in posts[i] which contains the psd_id.
               
                
            }
        }

    }
    global=id;
    return -1;
}



result* find_original(post* posts, size_t count, uint64_t post_id, query_helper* helper) {
    
    int temp;
    int flag=0;
     
        for(int i=0;i<count;i++){
            if(posts[i].pst_id==post_id){
               temp=i;
                flag=1;
                break;
            }     
        }
    if(flag==0){
        return NULL;
    }
    
    int updated_temp=helper_original(temp,posts,count);
    
    while(updated_temp!=-1){
        updated_temp=helper_original(updated_temp,posts,count);
         
    }
  
    //after jump out the recursive while statement,we malloc a heap containg 1 element and to be returned.
    result* return_origin=(result*)malloc(sizeof(result));
    return_origin->n_elements=1;
    post** element=(post**)malloc(sizeof(post*)*1);
   
    element[0]=&(posts[global]);
    return_origin->elements=(void**)element;

    return return_origin;
}

result* shortest_user_link(user* users, size_t count, uint64_t userA, uint64_t userB, query_helper* helper) {
 
    
    
    if(userA==userB){
        return NULL;
    }
//      for(int i=0;i<users[0].n_following;i++){
//         printf("%ld\n",users[0].following_idxs[i]);
//     }
//      printf("sos\n");
//     for(int i=0;i<users[2].n_following;i++){
//         printf("%ld\n",users[2].following_idxs[i]);
//     }
    
//      printf("sos\n");
//     for(int i=0;i<users[3].n_following;i++){
//         printf("%ld\n",users[3].following_idxs[i]);
//     }
    int print_result_1;
    int print_result_2;
    int index_array[count];
    // index_array[0]=0;
    int index_array_2[count];
    int index_1=0;
    int index_2=0;
    int user_pre[count];
    int user_pre_2[count];
    int flag[count];
    int flag_2[count];
    int user1=-2;
    int user2=-2;
    // int isfind_1=1;
    // int isfind_2=1;
    
//     // int count_b=0;
    int temp_user_flag;
    // int temp_user_flag_2;
    
//     // int shortest_route
	for(int i=0;i<count;i++){
        user_pre[i]=-1;
        user_pre_2[i]=-1;
        flag_2[i]=1;
        flag[i]=1;
        if(users[i].user_id==userA){
            user1=i;
        }
        if(users[i].user_id==userB){
            user2=i;
        }
    }

  
    if(user1==-2 || user2==-2){
        return NULL;
    }
//     // printf("%d\n",user1);
   
    queu *temp=createQueue(count);
    // printf("%d\n",user2);
    enqueue(temp,user2);
    // printf("%d    sos\n",temp->size);
    while(temp->size!=0){
        
        int temp_user_index=dequeue(temp);
        if(flag[temp_user_index]==1){
            flag[temp_user_index]=0;
        }
        for(int i=0;i<users[temp_user_index].n_following;i++){
            temp_user_flag=users[temp_user_index].following_idxs[i];
            
            // printf("%d\n",temp_user_flag);
            if(flag[temp_user_flag]==1){
                
                user_pre[temp_user_flag]=temp_user_index;
                flag[temp_user_flag]=0;
                enqueue(temp,temp_user_flag);
            }
            
        }
    }
        print_result_1=user1;//0
        while(user_pre[print_result_1]!=user2){
        if(user_pre[print_result_1]==-1){
            free(temp->array);
            free(temp);
            // isfind_1=0;
            result* return_origin=(result*)malloc(sizeof(result));
            return_origin->n_elements=0;
            user** element=NULL;
             return_origin->elements=(void**)element;
            // break;
            return return_origin;
        }
        index_array[index_1]=user_pre[print_result_1];
        index_1++;
        
        print_result_1=user_pre[print_result_1];
    }
    
    // printf("%d\n",index_1);
    
    ////////////////////////////////////////////////////////////////
     queu *temp2=createQueue(count);
    // printf("%d\n",user2);
    enqueue(temp2,user1);
    // printf("%d    sos\n",temp->size);
    while(temp2->size!=0){
        
        int temp_user_index_2=dequeue(temp2);
        if(flag_2[temp_user_index_2]==1){
            flag_2[temp_user_index_2]=0;
        }
        for(int i=0;i<users[temp_user_index_2].n_following;i++){
            temp_user_flag=users[temp_user_index_2].following_idxs[i];
            
            // printf("%d\n",temp_user_flag);
            if(flag_2[temp_user_flag]==1){
                
                user_pre_2[temp_user_flag]=temp_user_index_2;
                flag_2[temp_user_flag]=0;
                enqueue(temp2,temp_user_flag);
            }
            
        }
    }
//      print_result_1=user1;//0
//         while(user_pre[print_result_1]!=user2){
//         if(user_pre[print_result_1]==-1){
//             free(temp->array);
//     free(temp);
//             // isfind_1=0;
//             return NULL;
//             // break;
//         }
//         index_array[index_1]=user_pre[print_result_1];
//         index_1++;
        
//         print_result_1=user_pre[print_result_1];
//     }
    
    
    // printf("%d\n",user_pre_2[2]);
     print_result_2=user2;//0
        while(user_pre_2[print_result_2]!=user1){
        if(user_pre_2[print_result_2]==-1){
            free(temp2->array);
            free(temp2);
            free(temp->array);
            free(temp);
             result* return_origin=(result*)malloc(sizeof(result));
    return_origin->n_elements=0;
    user** element=NULL;
     return_origin->elements=(void**)element;
            // isfind_2=0;
            return return_origin;
            
        }
        index_array_2[index_2]=user_pre[print_result_2];
        index_2++;
        
        print_result_2=user_pre_2[print_result_2];
    }
    
 
    
//      if(isfind_2==0 && isfind_1==0){
//           free(temp->array);
//     free(temp);
//           free(temp2->array);
//     free(temp2);
    
    
//            return NULL;
//      }
    
    
    ///////////////////////////////////////////
    if(index_2>index_1){ 
    result* return_origin=(result*)malloc(sizeof(result));
    return_origin->n_elements=index_1+2;
    user** element=(user**)malloc(sizeof(user*)*(index_1+2));
    if(index_1!=0){
        for(int i=1;i<index_1+1;i++){
            element[i]=&users[index_array[i-1]];
        }
    }
     
    element[0]=&users[user2];
    element[1+index_1]=&users[user1];
    return_origin->elements=(void**)element;
     free(temp2->array);
    free(temp2);
    free(temp->array);
    free(temp);
    
    return return_origin;
    }
    if(index_2<=index_1){ 
    result* return_origin=(result*)malloc(sizeof(result));
    return_origin->n_elements=index_2+2;
    user** element=(user**)malloc(sizeof(user*)*(index_2+2));
    if(index_2!=0){
        for(int i=1;i<index_2+1;i++){
            element[i]=&users[index_array_2[i-1]];
        }
    }
     
    element[0]=&users[user1];
    element[1+index_2]=&users[user2];
    return_origin->elements=(void**)element;
     free(temp2->array);
    free(temp2);
    free(temp->array);
    free(temp);
    
    return return_origin;
    }
    return NULL;
}
    

int find_bot_sec_helper(int* array,int id,int count ){
    for(int i=0;i<count;i++){
        if(array[i]==id){
            return 1;
        }
    }
    return 0;
}

int find_bot_helper(int* array,int id,int count){
    for(int i=0;i<count;i++){
        if(array[i]==id){
            return 1;
        }
    }
    return 0;
}

int check_exist_already(int* previous,int id,int count){
    for(int i=0;i<count;i++){
        if(previous[i]==id){
            return 1;
        }
    }
    return 0;
}
result* find_bots(user* users, size_t user_count, post* posts, size_t post_count, criteria* crit, query_helper* helper) {
    // printf("%d\n",user_count);
	float oc_threshold=crit->oc_threshold;
	float acc_rep_threshold=crit->acc_rep_threshold;
	float bot_net_threshold=crit->bot_net_threshold;
    // printf("%f\n",bot_net_threshold);
	int arr_total_repostpool[post_count];
	int cursor=0;
	int sum=0;
    int sum_bot_again=0;
	float check_oc;
	int users_bot_pool[user_count];    //bot users id
    // int users_bot_sec_pool[user_count];
    // int cursor_user_second_bot=0;
	int cursor_user_bot=0;
    float check_last;
    float check_reputation;
    int check;
    
    // int cursor_user_fresh;
	for(int i=0;i<user_count;i++){
	for(int j=0;j<users[i].n_posts;j++){
        for(int k=0;k<posts[users[i].post_idxs[j]].n_reposted;k++){
            
        
	 arr_total_repostpool[cursor]=posts[users[i].post_idxs[j]].reposted_idxs[k];
	 cursor++;
	}
    }
	}
	// printf("%d\n",1);
	for(int i=0;i<user_count;i++){
	
	for(int j=0;j<users[i].n_posts;j++){
	sum=sum+find_bot_helper(arr_total_repostpool,users[i].post_idxs[j],cursor);
	
	// if(fabs(check_oc-oc_threshold)>0.001 || fabs(acc_rep_threshold-check_reputation)>0.001){
   
        
	}

        check_reputation=(float)(users[i].n_followers)/(float)(users[i].n_followers+users[i].n_following);
        check_oc=(float)sum/(float)users[i].n_posts;
	
      if( (check_oc>oc_threshold) || (acc_rep_threshold>check_reputation) ){
          // printf("%d\n",cursor_user_bot);
	users_bot_pool[cursor_user_bot]=i;
	cursor_user_bot++;
	  }
         
           
       sum=0;
      
    }
    // printf("before discrete%d\n",cursor_user_bot);

 
    for(int i=0;i<user_count;i++){
        for(int j=0;j<users[i].n_followers;j++){
            sum_bot_again=sum_bot_again+find_bot_sec_helper(users_bot_pool,users[i].follower_idxs[j],cursor_user_bot);
            
        }
        
       
         check_last=(float)sum_bot_again/(float)users[i].n_followers;
        
        if(bot_net_threshold<check_last){
            // printf("222222222\n");
            check=check_exist_already(users_bot_pool,i,cursor_user_bot);
            if(check==0){
                
                users_bot_pool[cursor_user_bot]=i;
                cursor_user_bot++;
                i=-1;
                // users_bot_sec_pool[cursor_user_second_bot]=i;
                // cursor_user_second_bot++;
            }
        }
        
        sum_bot_again=0;
        
    }
    // printf("%d\n",cursor_user_bot);
     
      // printf("after discrete%d\n",cursor_user_bot);
    result* return_origin=(result*)malloc(sizeof(result));
    return_origin->n_elements=cursor_user_bot;
    user** element=(user**)malloc(sizeof(user*)*(cursor_user_bot));
    for(int i=0;i<cursor_user_bot;i++){
        int temp=users_bot_pool[i];
        element[i]=&users[temp];
    }
    
    // for(int j=0;j<cursor_user_second_bot;j++){
    //     int temp=users_bot_sec_pool[j];
    //     element[j+cursor_user_bot]=&users[temp];
    // }
    // printf("%d\n",cursor_user_second_bot);
    
    return_origin->elements=(void**)element;
    return return_origin ;
    
}



void engine_cleanup(query_helper* helpers) {
    //Clean up your engine
}


