#pragma once

#include "../Logger/Logger.hpp"
#include "../EventBus/Event.hpp"

#include <map>
#include <typeindex>
#include <list>
#include <functional>

class IEventCallback {
    private:
        virtual void Call(Event& e) = 0;

    public:
        virtual ~IEventCallback() = default;

        void Execute(Event& e) {
            Call(e);
        }
};

template <typename TOwner, typename TEvent>
class EventCallback: public IEventCallback {
    private:
        using CallbackFunction = void (TOwner::*)(TEvent&);

        TOwner *ownerInstance;
        CallbackFunction callbackFunction;

        virtual void Call(Event& e) override {
            std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
        }

    public:
        EventCallback(TOwner *ownerInstance, CallbackFunction callbackFunction): ownerInstance(ownerInstance), callbackFunction(callbackFunction) {}
        virtual ~EventCallback() override = default;
};

using HandlerList = std::list<std::unique_ptr<IEventCallback>>;

class EventBus {
    private:
        std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

    public:
        EventBus() {
            Logger::Log("EventBus constructor called!");
        }

        ~EventBus() {
            Logger::Log("EventBus deconstructor called!");
        }

        // Clear subscribers.
        void Reset() { subscribers.clear(); }

        // Listener subscribes to an event: eventBus->SubscribeToEvent<CollisonEvent>(this, &Game::onCollision);
        template<typename TEvent, typename TOwner>
        void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)) {
            if (!subscribers[typeid(TEvent)].get()) {
                subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
            }
            auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
            subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
        }

        // As soon as something emits an event, we execute all listener callbacks: eventBus->EmitEvent<CollisonEvent>(player, entity);
        template<typename TEvent, typename ...TArgs>
        void EmitEvent(TArgs&& ...args) {
            auto handlers = subscribers[typeid(TEvent)].get();
            if (handlers) {
                for (auto it = handlers->begin(); it != handlers->end(); it++) {
                    auto handler = it->get();
                    TEvent event(std::forward<TArgs>(args)...);
                    handler->Execute(event);
                }
            }
        }
};

