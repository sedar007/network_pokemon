//
// Created by Adjy Sedar Desir on 02/02/2026.
//

#include "pch.h"

namespace pokemon {

     Thread_pool::Thread_pool(size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);

                        condition.wait(lock, [this] {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty())
                            return;

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    Thread_pool::~Thread_pool() {
         {
             std::unique_lock<std::mutex> lock(queueMutex);
             stop = true;
         }
         condition.notify_all();

         for(auto& worker : workers) {
             if(worker.joinable())
                 worker.join();
         }
    }
}
