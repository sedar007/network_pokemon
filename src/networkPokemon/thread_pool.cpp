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
                        // On verrouille pour accéder à la queue
                        std::unique_lock<std::mutex> lock(queueMutex);

                        // On attend qu'une tâche arrive ou qu'on nous dise d'arrêter
                        condition.wait(lock, [this] {
                            return stop || !tasks.empty();
                        });

                        // Si on doit arrêter et qu'il n'y a plus de tâches, on sort
                        if (stop && tasks.empty())
                            return;

                        // On prend la tâche
                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    // On exécute la tâche hors du verrou (pour ne pas bloquer les autres)
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
