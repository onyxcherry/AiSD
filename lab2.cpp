#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <numeric>
#include <ctime>
#include <iomanip>
#include <map>

std::random_device rd;
std::mt19937 rng(rd());

struct Node
{
    int key;
    Node *left;
    Node *right;

    Node(int key)
    {
        this->key = key;
        this->left = nullptr;
        this->right = nullptr;
    }
};

struct czas_droga
{
    std::clock_t czas;
    int droga;
};

std::vector<int> random_n_vec(int n)
{
    std::vector<int> v(n);
    iota(v.begin(), v.end(), 1);
    shuffle(v.begin(), v.end(), rng);
    return v;
}

std::vector<std::vector<std::vector<int>>> generate_tabs_random(int start, int stop, int step, int k)
{
    std::vector<std::vector<std::vector<int>>> tabs_random;
    for (int n{start}; n <= stop; n += step)
    {
        std::vector<std::vector<int>> list_n_changing;
        for (int i{0}; i < k; i++)
        {
            std::vector<int> random_n_numbers = random_n_vec(n);
            list_n_changing.push_back(random_n_numbers);
        }
        tabs_random.push_back(list_n_changing);
    }
    return tabs_random;
}

int q_part_middle(std::vector<int> &v, int left, int right)
{
    int pivot = v[(left + right) / 2];
    int p = left;
    int r = right;

    while (true)
    {
        while (v[p] < pivot)
        {
            p++;
        }
        while (v[r] > pivot)
        {
            r--;
        }

        if (p < r)
        {
            std::swap(v[p], v[r]);
            p++;
            r--;
        }
        else
        {
            if (r == right)
            {
                r--;
            }
            return r;
        }
    }
}

void quick_sort_middle(std::vector<int> &v, int left, int right)
{
    if (left < right)
    {
        int q = q_part_middle(v, left, right);
        quick_sort_middle(v, left, q);
        quick_sort_middle(v, q + 1, right);
    }
}

int my_binary_search(std::vector<int> &v, int l, int r, int x)
{
    while (l <= r)
    {
        int middle_idx = (l + r) / 2;
        int elem = v.at(middle_idx);
        if (elem == x)
        {
            return middle_idx;
        }
        if (elem < x)
        {
            l = middle_idx + 1;
        }
        else
        {
            r = middle_idx - 1;
        }
    }
    return -1;
}

int insert(Node *&node, int key, int h)
{
    if (node == nullptr)
    {
        node = new Node(key);
        return h;
    }
    else if (key < node->key)
    {
        return insert(node->left, key, h + 1);
    }
    else if (key > node->key)
    {
        return insert(node->right, key, h + 1);
    }
    else
    {
        throw 42;
    }
}

int insert_it(Node *&root, int key, int h)
{
    if (root == nullptr)
    {
        root = new Node(key);
        return h;
    }

    Node *parent = nullptr;
    Node *current = root;

    while (current != nullptr)
    {
        parent = current;
        h++;
        if (current->key <= key)
        {
            current = current->right;
        }
        else
        {
            current = current->left;
        }
    }

    if (parent->key <= key)
    {
        parent->right = new Node(key);
    }
    else
    {
        parent->left = new Node(key);
    }
    return h;
}

void empty(Node *&node)
{
    if (node != nullptr)
    {
        empty(node->left);
        empty(node->right);
        delete node;
    }
}

bool search(Node *node, int key)
{
    while (node != nullptr)
    {
        if (key == node->key)
        {
            return true;
        }
        else if (key < node->key)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }
    return false;
}

int height(Node *node)
{
    if (node == nullptr)
    {
        return -1;
    }
    int left_height = height(node->left);
    int right_height = height(node->right);
    return 1 + std::max(left_height, right_height);
}

double ex2_creation(std::vector<int> &A)
{
    std::clock_t cb_start = clock();

    std::vector<int> B(A);
    quick_sort_middle(B, 0, B.size() - 1);

    return clock() - cb_start;
}

double ex2_searching(std::vector<int> &A, std::vector<int> &B)
{
    long sa_start = std::clock();

    for (const auto b_elem : B)
    {
        for (auto elem : A)
        {
            if (elem == b_elem)
            {
                break;
            }
        }
    }

    return clock() - sa_start;
}

double ex2_binary_searching(std::vector<int> &A, std::vector<int> &B)
{
    std::clock_t sb_start = clock();
    for (const auto a_elem : A)
    {

        int idx = my_binary_search(B, 0, B.size() - 1, a_elem);
    }
    return clock() - sb_start;
}

double srednia_czasu(std::vector<double> &results)
{
    std::sort(results.begin(), results.end());
    double srednia = std::accumulate(results.begin() + 1, results.end() - 1, 0) / double(results.size() - 2);
    double wynik = srednia * 1000 / CLOCKS_PER_SEC;
    results.clear();
    return wynik;
}

void ex2(std::vector<std::vector<std::vector<int>>> &tabs_random)
{
    std::cout << "n\tC_B\tS_A\tS_B" << std::endl;
    for (auto list_n_changing : tabs_random)
    {
        int ile_elementow = list_n_changing[0].size();
        std::cout << ile_elementow << '\t';

        std::vector<double> results;

        for (auto A : list_n_changing)
        {
            double cb_duration = ex2_creation(A);
            results.push_back(cb_duration);
        }
        std::cout << srednia_czasu(results) << '\t' << std::flush;

        for (auto A : list_n_changing)
        {
            std::vector<int> B(A);
            std::sort(B.begin(), B.end());
            double sa_duration = 0;
            if (ile_elementow <= 20000)
            {
                sa_duration = ex2_searching(A, B);
            }
            results.push_back(sa_duration);
        }
        std::cout << srednia_czasu(results) << '\t';

        for (auto A : list_n_changing)
        {
            std::vector<int> B(A);
            std::sort(B.begin(), B.end());
            double sb_duration = ex2_binary_searching(A, B);
            results.push_back(sb_duration);
        }
        std::cout << srednia_czasu(results) << std::endl;
    }
    std::cout << std::endl;
}

czas_droga ex3_creation(std::vector<int> &A, Node *&root)
{
    std::clock_t c_ta_start = std::clock();
    for (int elem : A)
    {
        insert_it(root, elem, 0);
    }
    std::clock_t czas = clock() - c_ta_start;

    empty(root);

    root = nullptr;

    std::map<int, int> przebyte;
    for (int elem : A)
    {
        int h = insert_it(root, elem, 0);
        przebyte[h]++;
    }

    int suma = std::accumulate(przebyte.begin(), przebyte.end(), 0, [](int acc, std::pair<int, int> p)
                               { return acc + p.first * p.second; });
    czas_droga c_d = {czas, suma};
    return c_d;
}

double ex3_searching(std::vector<int> &A, Node *&root)
{
    std::clock_t s_ta_start = std::clock();
    for (int elem : A)
    {
        search(root, elem);
    }
    return clock() - s_ta_start;
}

void ex3(std::vector<std::vector<std::vector<int>>> &tabs_random)
{
    std::cout << "n\tC_TA\tdis_TA\th_TA\tS_TA" << std::endl;

    for (auto list_n_changing : tabs_random)
    {
        int ile_elementow = list_n_changing[0].size();
        std::cout << ile_elementow << '\t';

        std::vector<double> czasy;
        std::vector<int> drogi;
        std::vector<Node *> roots;

        for (auto A : list_n_changing)
        {
            Node *root = nullptr;
            czas_droga ca_czasdroga = ex3_creation(A, root);

            roots.push_back(root);
            czasy.push_back(ca_czasdroga.czas);
            drogi.push_back(ca_czasdroga.droga);
        }

        std::cout << srednia_czasu(czasy) << '\t';
        std::cout << std::accumulate(drogi.begin(), drogi.end(), 0) / drogi.size() << '\t';
        czasy.clear();
        drogi.clear();

        std::vector<int> tree_heights;
        for (auto root : roots)
        {
            int tree_height = height(root);
            tree_heights.push_back(tree_height);
        }
        std::sort(tree_heights.begin(), tree_heights.end());
        std::cout << std::accumulate(tree_heights.begin() + 1, tree_heights.end() - 1, 0) / double(tree_heights.size() - 2) << '\t';
        tree_heights.clear();

        czasy.clear();
        int i{0};
        for (auto A : list_n_changing)
        {
            Node *root = roots.at(i);
            double ca_searching = ex3_searching(A, root);
            czasy.push_back(ca_searching);
            empty(root);
            i++;
        }
        std::cout << srednia_czasu(czasy) << std::endl;
    }
    std::cout << std::endl;
}

void optimum_input_binary(std::vector<int> &C, int l, int r)
{
    if (l > r)
    {
        return;
    }
    int middle_idx = (l + r) / 2;
    C.push_back(middle_idx);
    optimum_input_binary(C, l, middle_idx - 1);
    optimum_input_binary(C, middle_idx + 1, r);
}

void ex4(std::vector<std::vector<std::vector<int>>> &tabs_random)
{
    std::cout << "n\tC_TB\tdis_TB\th_TB\tS_TB" << std::endl;

    for (auto list_n_changing : tabs_random)
    {
        int ile_elementow = list_n_changing[0].size();
        std::cout << ile_elementow << '\t';

        std::vector<double> czasy;
        std::vector<int> drogi;
        std::vector<Node *> roots;

        for (auto A : list_n_changing)
        {
            std::vector<int> C;
            optimum_input_binary(C, 1, A.size());

            Node *root = nullptr;
            czas_droga ca_czasdroga = ex3_creation(C, root);
            roots.push_back(root);
            czasy.push_back(ca_czasdroga.czas);
            drogi.push_back(ca_czasdroga.droga);
        }
        std::cout << srednia_czasu(czasy) << '\t';
        std::cout << std::accumulate(drogi.begin(), drogi.end(), 0) / drogi.size() << '\t';
        czasy.clear();
        drogi.clear();

        std::vector<int> tree_heights;
        for (auto root : roots)
        {
            int tree_height = height(root);
            tree_heights.push_back(tree_height);
        }
        std::sort(tree_heights.begin(), tree_heights.end());
        std::cout << std::accumulate(tree_heights.begin() + 1, tree_heights.end() - 1, 0) / double(tree_heights.size() - 2) << '\t';
        tree_heights.clear();

        czasy.clear();
        int i{0};
        for (auto A : list_n_changing)
        {
            Node *root = roots.at(i);
            double ca_searching = ex3_searching(A, root);
            czasy.push_back(ca_searching);
            empty(root);
            i++;
        }
        std::cout << srednia_czasu(czasy) << std::endl;
    }
    std::cout << std::endl;
}

void wysokosci()
{
    std::cout << "n" << '\t' << "h_TA" << '\t' << "h_TB" << std::endl;

    std::vector<std::vector<std::vector<int>>> tabs_random;
    for (int n{50}; n <= 750; n += 50)
    {
        std::vector<std::vector<int>> list_n_changing;
        for (int i{0}; i < 20; i++)
        {
            std::vector<int> random_n_numbers = random_n_vec(n);
            list_n_changing.push_back(random_n_numbers);
        }
        tabs_random.push_back(list_n_changing);
    }

    std::vector<int> tree_heights;
    for (auto list_n_changing : tabs_random)
    {
        std::vector<int> losowe;
        std::vector<int> ulozone;

        for (auto A : list_n_changing)
        {
            Node *root1 = nullptr;
            ex3_creation(A, root1);
            int tree_height1 = height(root1);
            losowe.push_back(tree_height1);
            empty(root1);

            std::vector<int> C;
            optimum_input_binary(C, 1, A.size());

            Node *root2 = nullptr;
            ex3_creation(C, root2);
            int tree_height2 = height(root2);
            ulozone.push_back(tree_height2);
            empty(root2);
        }

        std::cout << list_n_changing[0].size() << '\t';

        std::sort(losowe.begin(), losowe.end());
        std::cout << std::accumulate(losowe.begin() + 3, losowe.end() - 3, 0) / double(losowe.size() - 6) << '\t';

        std::sort(ulozone.begin(), ulozone.end());
        std::cout << std::accumulate(ulozone.begin() + 3, ulozone.end() - 3, 0) / double(ulozone.size() - 6) << std::endl;
    }
}

void przebyte_drogi()
{
    std::cout << "n\tTA\tTB" << std::endl;
    auto tabs_random = generate_tabs_random(10000, 150000, 10000, 5);
    std::vector<int> tree_heights;
    for (auto list_n_changing : tabs_random)
    {
        int ile_elementow = list_n_changing[0].size();
        std::cout << ile_elementow << '\t';

        std::vector<int> losowe;
        std::vector<int> ulozone;

        for (auto A : list_n_changing)
        {
            Node *root1 = nullptr;
            czas_droga cd1 = ex3_creation(A, root1);
            losowe.push_back(cd1.droga);
            empty(root1);

            std::vector<int> C;
            optimum_input_binary(C, 1, A.size());

            Node *root2 = nullptr;
            czas_droga cd2 = ex3_creation(C, root2);
            ulozone.push_back(cd2.droga);
            empty(root2);
        }
        std::cout << std::accumulate(losowe.begin(), losowe.end(), 0) / double(losowe.size() * ile_elementow) << '\t';
        std::cout << std::accumulate(ulozone.begin(), ulozone.end(), 0) / double(losowe.size() * ile_elementow) << std::endl;
        // std::cout << losowe[losowe.size() / 2] / double(ile_elementow) << '\t';
        // std::cout << ulozone[ulozone.size() / 2] / double(ile_elementow) << std::endl;
    }
}

int main()
{
    // std::cout.imbue(std::locale("en_US.UTF-8"));
    std::cout << "CLOCKS_PER_SEC=" << CLOCKS_PER_SEC << std::setprecision(2) << std::fixed << std::endl;
    std::cout << "Precyzja: " << std::cout.precision() << std::endl
              << std::endl;
    std::vector<std::vector<std::vector<int>>> tabs_random;
    for (int n{10000}; n <= 150000; n += 10000)
    {
        std::vector<std::vector<int>> list_n_changing;
        for (int i{0}; i < 10; i++)
        {
            std::vector<int> random_n_numbers = random_n_vec(n);
            list_n_changing.push_back(random_n_numbers);
        }
        tabs_random.push_back(list_n_changing);
    }

    ex2(tabs_random);
    ex3(tabs_random);
    ex4(tabs_random);
}

