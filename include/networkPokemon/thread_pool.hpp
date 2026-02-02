#pragma once

//
// Created by Adjy Sedar Desir on 02/02/2026.
//

#ifndef NETWORKPOKEMON_THREADPOOL_H
#define NETWORKPOKEMON_THREADPOOL_H

namespace pokemon {
    class NETWORK_POKEMON_API Thread_pool {
    public:
        explicit Thread_pool(size_t numThreads = std::thread::hardware_concurrency());
        ~Thread_pool();

        // F can be any callable type
        template<class F>
        void enqueue(F&& f) {
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                tasks.emplace(std::forward<F>(f));
            }
            condition.notify_one(); // Notify one worker thread
        }

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;

        std::mutex queueMutex;
        std::condition_variable condition;
        bool stop = false;
    };
}


#endif //NETWORKPOKEMON_THREADPOOL_H
