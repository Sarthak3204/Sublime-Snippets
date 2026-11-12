// @aliases stack_queues_trap_rain_water2
int trap(vector<int>& height) {
    int n = height.size(), ans = 0, l = 0, r = n - 1, maxL = 0, maxR = 0;

    while (l <= r) {
        if (height[l] <= height[r]) {
            if (maxL >= height[l]) ans += maxL - height[l];
            else maxL = height[l];
            l++;
        }
        else {
            if (maxR >= height[r]) ans += maxR - height[r];
            else maxR = height[r];
            r--;
        }
    }

    return ans;
}
