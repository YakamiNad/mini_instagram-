# mini_instagram-
The social media platform is composed of two types of entities:
• Users • Posts
  Each user in the social network have a list of followers, other users they are following and posts. These collections are represented as an array of indices that relate to a user* or post* array.
struct user {
  uint64_t user_id; //User id, is unique  /n
  size_t* followers_idxs; //User indices within user* users graph size_t n_followers; //Number of followers /n
  size_t* following_idxs; //User indices within user* users graph size_t n_following; //Number of users the user is following   size_t* posts_idxs; //Post indices within post* posts graph size_t n_posts; // Number of posts a user has made.
};
typedef struct user user;
All user_id values within the users array will be unique. The lists: followers, following and posts are arranged by ascending order. In order of insertion.
struct post {
uint64_t pst_id; //Post id, is unique
size_t* reposted_idxs; //Reposted indices within the graph size_t n_reposted; // Number of reposted posts.
};
typedef struct post post;
All pst_id values within the posts array will be unique. The posts array will not contain any cycles as for any reposts to exist depends on the existance of an original post.
For all queries you will need to return a heap allocated result structure: This structure stores all elements retrieved from the query as a heap allocated contiguous block of pointers and the number of elements. It will be used to determine if your result is correct.
struct result { void** elements; size_t n_elements;
};
typedef struct result result;


The entire social network structure is an array of users and an array of posts. Both elements contain links to each other as well as to themselves.
