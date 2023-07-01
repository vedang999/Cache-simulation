#include <bits/stdc++.h>

using namespace std;
//this one by gpt
class LRUCache {
    class Node {
    public:
        int key;
        int value;
        Node* next;
        Node* prev;
        
        Node(int k, int v) {
            key = k;
            value = v;
            next = nullptr;
            prev = nullptr;
        }
    };
    unordered_map<int, Node*> mp;
    int capacity;
    Node* head;
    Node* tail;
    
public:
    
    LRUCache(int capacity) {
        this->capacity = capacity;
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }
    
    int get(int key) {
        if (mp.find(key) == mp.end())
            return -1;
        
        Node* node = mp[key];
        moveToHead(node);
        return node->value;
    }
    
    void put(int key, int value) {
        if (mp.find(key) != mp.end()) {
            Node* node = mp[key];
            node->value = value;
            moveToHead(node);
        } else {
            Node* newNode = new Node(key, value);
            mp[key] = newNode;
            addToHead(newNode);
            
            if (mp.size() > capacity) {
                Node* tailNode = removeTail();
                mp.erase(tailNode->key);
                delete tailNode;
            }
        }
    }
    void clearCache() {
    // Traverse the cache and delete all nodes
    Node* currentNode = head->next;
    while (currentNode != tail) {
        Node* nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
    }

    // Reset the cache data structures
    mp.clear();
    head->next = tail;
    tail->prev = head;
}

private:
    void moveToHead(Node* node) {
        removeNode(node);
        addToHead(node);
    }
    
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    void addToHead(Node* node) {
        node->next = head->next;
        node->next->prev = node;
        node->prev = head;
        head->next = node;
    }
    
    Node* removeTail() {
        Node* tailNode = tail->prev;
        removeNode(tailNode);
        return tailNode;
    }
};

class LFUCache {
    class node {
    public:
        int key;
        int value;
        int count;
        node* next;
        node* prev;

        node(int k, int v) {
            key = k;
            value = v;
            count = 1;
            next = NULL;
            prev = NULL;
        }
    };

    unordered_map<int, node*> mp;
    unordered_map<int, pair<node*, node*>> freq; // key to head, tail
    int cap;
    int global_min_freq;

void shift1(node* N) {
    int ccnt = N->count; // current count
    
    // remove N
    node* head = freq[ccnt].first;
    node* tail = freq[ccnt].second;
    // adjust global freq min 
    if ((global_min_freq == ccnt) && (head == tail))global_min_freq=ccnt+1;

    if(head==tail){
        head=NULL;
        tail=NULL;
    }
    else if (N == tail) {
        tail = tail->prev;
        tail->next = NULL;
    }
    else if (N == head) {
        head = head->next;
        if(head!=NULL)head->prev = NULL;
    }
    else {
        N->prev->next = N->next;
        N->next->prev = N->prev;
    }
    freq[ccnt]=make_pair(head,tail);

    N->next = NULL;
    N->prev = NULL;


    // insert at next freq
    ccnt++;
    N->count = ccnt;

    node* head1 = freq[ccnt].first;
    node* tail1 = freq[ccnt].second;

    if (head1 != NULL) {
        N->next = head1;
        head1->prev = N;
    }
    head1 = N;
    if (tail1 == NULL)
        tail1 = N;

    freq[ccnt] = make_pair(head1, tail1); // Update freq map with new head and tail pointers
}


    void insert_at_1(int k, int d) {
        node* N = new node(k, d);
        mp[k] = N;

        node* head = freq[1].first;
        node* tail = freq[1].second;

        if (head != NULL) {
            N->next = head;
            head->prev = N;
        }

        head = N;

        if (tail == NULL)
            tail = N;
        freq[1]=make_pair(head,tail);
        global_min_freq = 1;
    }

    void removetail() {
        node* head = freq[global_min_freq].first;
        node* tail = freq[global_min_freq].second;

        mp.erase(tail->key);
        if (tail->prev != NULL) {
            tail = tail->prev;
            delete (tail->next);
        }
        else {
            delete tail;
            head = NULL;
            tail = NULL;
        }
        freq[global_min_freq]=make_pair(head,tail);
    }
public:
    LFUCache(int capacity) {
        cap = capacity;
        std::pair<node*, node*> p;
        p.first = NULL;
        p.second = NULL;
        freq[1] = p;
        global_min_freq = 0;
    }

    int get(int k) {
        if (mp.find(k) == mp.end())
            return -1;
        
        shift1(mp[k]);
        return mp[k]->value;
    }

    void put(int k, int d) {
        if (mp.find(k) != mp.end()) {
            if(mp[k]->value==d)return;
            mp[k]->value = d;
            shift1(mp[k]);
        }
        else if (mp.size() < cap) {
            insert_at_1(k, d);
        }
        else {
            removetail();
            insert_at_1(k, d);
        }
    }
        void clearCache() {
        for (auto it = freq.begin(); it != freq.end(); ++it) {
            node* head = it->second.first;
            node* curr = head;

            while (curr) {
                node* next = curr->next;
                delete curr;
                curr = next;
            }
        }

        mp.clear();
        freq.clear();
        global_min_freq = 0;
    }
};

class MRUCache {
class Node {
public:
    int key;
    int value;
    Node* next;
    Node* prev;

    Node(int key, int value) : key(key), value(value), next(nullptr), prev(nullptr) {}
};
private:
    int capacity;
    std::unordered_map<int, Node*> cache;
    Node* head;
    Node* tail;

public:
    MRUCache(int capacity) : capacity(capacity), head(nullptr), tail(nullptr) {}

    int get(int key) {
        if (cache.find(key) != cache.end()) {
            Node* node = cache[key];
            moveNodeToFront(node);
            return node->value;
        }
        else {
            return -1;
        }
    }

    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            Node* node = cache[key];
            node->value = value;
            moveNodeToFront(node);
        }
        else {
            if (cache.size() == capacity) {
                removeHead();
            }
            Node* newNode = new Node(key, value);
            cache[key] = newNode;
            addNodeToFront(newNode);
        }
    }

    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        cache.clear();
        head = nullptr;
        tail = nullptr;
    }

private:
    void moveNodeToFront(Node* node) {
        if (node == head) {
            return;
        }

        node->prev->next = node->next;
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        }
        else {
            tail = node->prev;
        }

        addNodeToFront(node);
    }

    void addNodeToFront(Node* node) {
        if (head == nullptr) {
            head = node;
            tail = node;
        }
        else {
            node->next = head;
            head->prev = node;
            head = node;
        }
    }

    void removeHead() {
        cache.erase(head->key);

        Node* temp = head;
        head = head->next;

        if (head != nullptr) {
            head->prev = nullptr;
        }
        else {
            tail = nullptr;
        }

        delete temp;
    }
};

class FIFOCache {
private:
    int capacity;
    std::unordered_map<int, int> cache;
    std::queue<int> fifo;

public:
    FIFOCache(int capacity) : capacity(capacity) {}

    int get(int key) {
        if (cache.find(key) != cache.end()) {
            return cache[key];
        } else {
            return -1;
        }
    }

    void put(int key, int value) {
        if (cache.size() == capacity && cache.find(key) == cache.end()) {
            int oldestKey = fifo.front();
            fifo.pop();
            cache.erase(oldestKey);
        }

        cache[key] = value;
        fifo.push(key);
    }

    void clear() {
        cache.clear();
        while (!fifo.empty()) {
            fifo.pop();
        }
    }
};

class RandomCache {
private:
    int capacity;
    std::unordered_map<int, int> cache;

public:
    RandomCache(int capacity) : capacity(capacity) {
        srand(time(nullptr));//srand() is a function that sets the seed for the random number generator used by functions like rand()
    }

    int get(int key) {
        if (cache.find(key) != cache.end()) {
            return cache[key];
        }
        return -1;
    }

    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            cache.erase(key);
        } else if (cache.size() >= capacity) {
            int randomIndex = rand() % cache.size();
            auto it = cache.begin();
            advance(it, randomIndex);
            cache.erase(it);
        }
        cache[key] = value;
    }

    void clear() {
        cache.clear();
    }
};

int main(){
cout<<"LRU cache"<<endl;

    LRUCache* lRUCache = new LRUCache(2);
    lRUCache->put(1, 1);
    lRUCache->put(2, 2);
    cout<<lRUCache->get(1)<<endl;
    lRUCache->put(3, 3);
    cout<<lRUCache->get(2)<<endl;
    lRUCache->put(4, 4);
    cout<<lRUCache->get(1)<<endl;
    cout<<lRUCache->get(3)<<endl;
    cout<<lRUCache->get(4)<<endl; 

    lRUCache->clearCache();

    lRUCache->put(1, 1);
    lRUCache->put(2, 2);
    cout<<lRUCache->get(1)<<endl;
    lRUCache->put(3, 3);
    cout<<lRUCache->get(2)<<endl;
    lRUCache->put(4, 4);
    cout<<lRUCache->get(1)<<endl;
    cout<<lRUCache->get(3)<<endl;
    cout<<lRUCache->get(4)<<endl;

    delete lRUCache;
cout<<"LFU cache"<<endl;

    LFUCache* lfu = new LFUCache(2);
    lfu->put(1, 1);
    lfu->put(2, 2);
    cout<<lfu->get(1)<<endl;
    lfu->put(3, 3);
    cout<<lfu->get(2)<<endl;
    cout<<lfu->get(3)<<endl;
    lfu->put(4, 4);
    cout<<lfu->get(1)<<endl;
    cout<<lfu->get(3)<<endl;
    cout<<lfu->get(4)<<endl;

    lfu->clearCache();

    lfu->put(1, 1);
    lfu->put(2, 2);
    cout<<lfu->get(1)<<endl;
    lfu->put(3, 3);
    cout<<lfu->get(2)<<endl;
    cout<<lfu->get(3)<<endl;
    lfu->put(4, 4);
    cout<<lfu->get(1)<<endl;
    cout<<lfu->get(3)<<endl;
    cout<<lfu->get(4)<<endl;

    delete lfu;
cout<<"MRU cache"<<endl;

    MRUCache* cachePtr = new MRUCache(3);
    cachePtr->put(1, 10);
    cachePtr->put(2, 20);
    cachePtr->put(3, 30);
    cout <<cachePtr->get(2)<< endl;
    cout <<cachePtr->get(4)<< endl;
    cachePtr->put(4, 40);
    cachePtr->put(5,50);
    cout <<cachePtr->get(2)<< endl;
    cachePtr->clear();
    delete cachePtr;
cout<<"FIFO cache"<<endl;

    FIFOCache* fifoobj = new FIFOCache(3);
    fifoobj->put(1, 10);
    fifoobj->put(2, 20);
    fifoobj->put(3, 30);
    cout << fifoobj->get(2) <<endl;  // returns 20
    fifoobj->put(4, 40);
    cout << fifoobj->get(1) <<endl;
    fifoobj->clear();
    delete fifoobj;

cout<<"Random cache"<<endl;

    RandomCache* ranobj = new RandomCache(3);
    ranobj->put(1, 100);
    ranobj->put(2, 200);
    ranobj->put(3, 300);
    cout << ranobj->get(1) << endl;  // Output: 100
    cout << ranobj->get(2) << endl;  // Output: 200
    cout << ranobj->get(3) << endl;  // Output: 300
    cout << ranobj->get(4) << endl;  // Output: -1 (key does not exist)

    ranobj->put(5, 50);
    cout << ranobj->get(1) << endl;  // Output: 100
    cout << ranobj->get(2) << endl;  // Output: 200
    cout << ranobj->get(3) << endl;  // Output: 300
    ranobj->put(4, 400);
    cout << ranobj->get(1) << endl;  // Output: -1 (evicted)
    cout << ranobj->get(4) << endl;  // Output: 400

    ranobj->clear();
    delete ranobj;

    return 0;
}