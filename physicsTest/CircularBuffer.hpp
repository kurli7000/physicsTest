#ifndef CircularBuffer_hpp
#define CircularBuffer_hpp

#include <stdio.h>

template <typename T>
class CircularBuffer
{
public:
    struct CircularBufferData
    {
        T data;
        CircularBufferData* previous;
        CircularBufferData* next;
    };

    CircularBuffer(int size)
    {
        CircularBufferData* tail;
        
        for (int i = 0; i < size; i++)
        {
            CircularBufferData* data = new CircularBufferData;
            
            if (i == 0)
            {
                head = data;
            }
            else
            {
                data->previous = tail;
                tail->next = data;
            }
            
            tail = data;
        }
        
        // 1st element's previous = tail
        head->previous = tail;
        
        // last element's next = head
        tail->next = head;
    }

    T back()
    {
        assert(head != nullptr);
        return head->data;
    }

    void push_back(T data)
    {
        assert(head != nullptr);
        head->data = data;
        forward();
    }
    
    void forward()
    {
        assert(head->next != nullptr);
        head = head->next;
    }

    void backward()
    {
        assert(head->previous != nullptr);
        head = head->previous;
    }

private:
    CircularBufferData* head;
};

#endif /* CircularBuffer_hpp */
