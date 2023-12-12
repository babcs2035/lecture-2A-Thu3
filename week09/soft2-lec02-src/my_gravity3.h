// シミュレーション条件を格納する構造体
typedef struct condition
{
    int width;  // 見えている範囲の幅
    int height; // 見えている範囲の高さ
    double G;   // 重力定数
    double dt;  // シミュレーションの時間幅
} Condition;

// 個々の物体を表す構造体
typedef struct object
{
    double m;
    double x;
    double vx;
    double y;
    double vy;
    double prev_vx;
    double prev_vy;
    int flag;
} Object;

void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond);
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond);
void my_update_positions(Object objs[], const size_t numobj, const Condition cond);
