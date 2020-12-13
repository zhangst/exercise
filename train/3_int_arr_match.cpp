// 求三个整形数组的交集 2020-11-05

#include <stdio.h>

#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;


void vector_print(vector<int> & vec) {
    for (int i : vec) {
        printf("%d ", i);
    }
    printf("\n");
}

vector<int> array_intersection(vector<int> & v1, vector<int> & v2, vector<int> & v3) {
    vector<int> v12;
    set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v12));

    vector<int> res;
    set_intersection(v12.begin(), v12.end(), v3.begin(), v3.end(), back_inserter(res));

    return res;
}


void insert_map(map<int,int> & m_int, vector<int> & v_int) {
    set<int> s_int;

    for (auto i : v_int) {
        if (s_int.find(i) == s_int.end()) {
            s_int.insert(i);
            m_int[i]++;
        }
    }
}
vector<int> array_intersection_1(vector<int> & v1, vector<int> & v2, vector<int> & v3) {
    map<int, int> m_int;

    insert_map(m_int, v1);
    insert_map(m_int, v2);
    insert_map(m_int, v3);

    vector<int> res;
    for (auto & it : m_int) {
        if (it.second == 3) {
            res.push_back(it.first);
        }
    }

    return res;
}




int main(void) {
    vector<int> v_int1{1,2,3,4,5};
    vector<int> v_int2{2,3,4,6,7,7,7,8};
    vector<int> v_int3{2,4,9,10};

    
    printf("input:\n");
    vector_print(v_int1);
    vector_print(v_int2);
    vector_print(v_int3);

    
    printf("\nv1 output:\n");
    vector<int> result = array_intersection(v_int1, v_int2, v_int3);
    vector_print(result);



    printf("\nv2 output:\n");
    result = array_intersection_1(v_int1, v_int2, v_int3);
    vector_print(result);

    return 0;
}
