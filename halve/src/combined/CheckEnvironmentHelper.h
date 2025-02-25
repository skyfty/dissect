#ifndef CHECKENVIRONMENTHELPER_H
#define CHECKENVIRONMENTHELPER_

class CheckEnvironmentHelper {
public:
    CheckEnvironmentHelper(quint64 flags)
        : m_flags(flags) {
    }

    CheckEnvironmentHelper& operator()(quint64 f, bool s) {
        if (s) {
            m_flags |= f;
        }  else {
            m_flags &= ~f;
        }
        return *this;
    }

    operator quint64() const {
        return m_flags;
    }

private:
    quint64 m_flags;
};

#endif // CHECKENVIRONMENTHELPER_H
