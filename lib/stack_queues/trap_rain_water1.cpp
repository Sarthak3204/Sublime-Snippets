// @aliases stack_queues_trap_rain_water1
int trap(vector<int>& height) {
        int n = height.size();
        vector<int> pfmax = height, sfmax = height;

        for (int i = 1; i < n; i++)
            pfmax[i] = max(pfmax[i], pfmax[i - 1]);

        for (int i = n - 2; i >= 0; i--)
            sfmax[i] = max(sfmax[i], sfmax[i + 1]);

        int ans = 0;
        for (int i = 1; i + 1 < n; i++)
            ans += max<int>(0, min(pfmax[i - 1], sfmax[i + 1]) - height[i]);

        return ans;
    }
