/**
 * The FixedQueue is a circular FIFO queue that can hold up to a fixed
 * sized number of elements. Each instance of the queue will have the same
 * max capacity, but the user can provide an artifical limit.
 *
 * The queue will also be able to support overridding old data when the
 * capactiy is reached.
 *
 * NOTE: The elements of the queue must implement the copy assignment
 * operator.
 *
 * TODO: Consider locking mechanism around critical regions.
 *
 * @author Collin Bolles
 */
#ifndef _EVT_FIXED_QUEUE_
#define _EVT_FIXED_QUEUE_

#include <stdint.h>

namespace EVT::core::types
{

template<size_t maxSize, class Element>
class FixedQueue {
public:

    /**
     * Makes a queue without the need for dynamic memory allocation. The
     * queue will start out as empty.
     *
     * @param withOverwrite If true, when the queue is full, the oldest
     *      data is overwritten when another element is added.
     *      Defaults to false
     */
    FixedQueue(bool withOverwrite = false) {
        this->maxCapacity = maxSize;
        this->front = &elements[0];
        this->back = &elements[0];
        this->end = &elements[maxCapacity];
        this->size = 0;
        this->withOverwrite = withOverwrite;
    }

    /**
     * Add an element to the back of the queue. If the queue is full and
     * overwritting is not enabled, then a value of false will be returned.
     * In all other cases true is returned.
     *
     * Elements will be COPIED into the queue, as such it is expected each
     * element's class implements the copy assignment operator.
     *
     * @param element The element to add to the queue
     * @return True if the element was added successfully, false if the queue
     *      is full and overwritting is disabled.
     */
    bool append(Element& element) {
        if(size == maxCapacity && !withOverwrite)
            return false;

        // Assumed use of copy assignment operator
        *back = element;
        back++;
        if (back > end)
            back = &elements[0];

        // Check if overwritting took place
        if (size == maxCapacity) {
            front++;
            if (front > end)
                front = &elements[0];
        } else {
            size++;
        }

        return true;
    }

    /**
     * Get the first element in the queue. Doing so will effectively remove
     * that element from the queue. The value at the front of the queue will
     * be copied into the provided value.
     *
     * @param element The element to populate with the value from the queue.
     *      If the queue is empty, this element will not be changed.
     * @return True if an element was successfully removed from the queue,
     *      false if there are no elements in the queue.
     */
    bool pop(Element* element) {
        if (size == 0)
            return false;

        *element = *front;
        front++;
        if (front > end)
            front = &elements[0];
        size--;
        return true;
    }

    /**
     * Get if the queue is full. Note: This will return true if the size is
     * equal to the capacity. So even if overwritting is enabled, if
     * the size is the same as the capacty, then true will be turned.
     *
     * @return True if the queue is full, false otherwise.
     */
    bool isFull() {
        return size == maxCapacity;
    }

    /**
     * Get is the queue is empty.
     *
     * @return True if the queue is empty, false otherwise.
     */
    bool isEmpty() {
        return size == 0;
    }

    /**
     * Check to see if an element can be inserted. If overwritting is enabled,
     * this is always true. If overwritting is not enabled, then insertion
     * can only take place if the queue is not full.
     *
     * @return True if the queue is not full or if overwritting is enabled.
     */
    bool canInsert() {
        return withOverwrite || !isFull();
    }


private:
    /** The elements stored in the queue */
    Element elements[maxSize];
    /**
     * The max number of elements that are stored (can be any value between
     * 0 and QUEUE_MAX_CAPACITY).
     */
    size_t maxCapacity;
    /** Front of the queue, position of the next element to be removed */
    Element* front;
    /** Back of the queue,  position of the next element to be */
    Element* back;
    /** Handy variable which represents the end of the data buffer */
    Element* end;
    /** The number of elements currently stored */
    size_t size;
    /** Represents if oldest data is allowed to be overwritten when full */
    bool withOverwrite;

};

}  // namespace EVT::core::types

#endif
