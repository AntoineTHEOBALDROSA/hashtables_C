This C file is an implementation of a hashmap.

Features

    Chaining for Collisions: This hashmap uses linked lists to handle collisions.
    Basic Operations: Supports insertion in O(1) worst case, deletion in O(1) average and search in O(1) average.
    Hash function : It uses universal hashing with an universal set for hashing functions, ensuring an average O(1) time complexity for search and deletion operations no matter the input.

Using this hashtable, you beat 98% of people on Leetcode's Two-Sum time complexity for C langage. (The structure needs to be slightly modified though).

Reference : CLRS (Third Edition)
