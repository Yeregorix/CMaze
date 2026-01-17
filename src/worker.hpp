/*
* Copyright (c) 2026 Hugo Dupanloup (Yeregorix)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef WORKER_HPP
#define WORKER_HPP

#include <QObject>
#include <QRunnable>

struct WorkerParameters {
    int seed;
    int width, height;
    double errorFactor;
    int pathSize, wallSize;
    QString fileName;
};

constexpr int WORKER_MAX_PROGRESS = 1000;

class Worker : public QObject, public QRunnable {
    Q_OBJECT
    public:

    explicit Worker(const WorkerParameters &parameters);

    void run() override;

    bool isCancelled() const;

    signals:

    void message(const QString &message);

    void progress(int value);

    void finished();

    public slots:

    void cancel();

    private:

    const WorkerParameters _parameters;
    bool _cancelled{false};
};


#endif //WORKER_HPP