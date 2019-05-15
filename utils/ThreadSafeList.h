#pragma once

#include <list>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename T>
class ThreadSafeList
{
private:

    mutable std::mutex mMut;

    std::condition_variable mCond;

    std::list<T> mDataList;


public:

    ThreadSafeList()
    {
        //Empty
    }

    ThreadSafeList(const ThreadSafeList& aOther)
    {
        std::lock_guard<std::mutex> lOtherLock(aOther.mMut);
        std::lock_guard<std::mutex> lThisLock(mMut);

        mDataList = aOther;
    }

    ThreadSafeList& operator=(const ThreadSafeList& aOther)
    {
        std::lock_guard<std::mutex> lOtherLock(aOther.mMut);
        std::lock_guard<std::mutex> lThisLock(mMut);

        if (this != &aOther) {
            mDataList = aOther;
        }

        return *this;
    }

    ThreadSafeList(ThreadSafeList&& aOther)
    {
        std::lock_guard<std::mutex> lOtherLock(aOther.mMut);
        std::lock_guard<std::mutex> lThisLock(mMut);

        mDataList.swap(aOther.mDataList);
    }


    void PushBack(T aData)
    {
        std::lock_guard<std::mutex> lk(mMut);
        mDataList.push_back(aData);
        mCond.notify_one();
    }

    void WaitAndPopFront(T& returnedData)
    {
        std::unique_lock<std::mutex> lk(mMut);
        mCond.wait(lk, [this]{return !mDataList.empty();});
        returnedData = mDataList.front();
        mDataList.pop_front();
    }

    bool TryToPopFront(T& returnedData)
    {  
        std::lock_guard<std::mutex> lk(mMut);
        if(mDataList.empty())
        {
            return false;
        }
        else
        {
            returnedData = mDataList.front();
            mDataList.pop_front();
            return true;
        }
    }

    bool IsEmpty() const
    {
        std::lock_guard<std::mutex> lk(mMut);
        return mDataList.empty();
    }

    size_t Size() const
    {
        std::lock_guard<std::mutex> lk(mMut);
        return mDataList.size();
    }  

    void notifyAll(T zeroToPush)
    {
        //std::lock_guard<std::mutex> lk(mMut);
        mDataList.push(zeroToPush);
        mCond.notify_all();
    }
};
